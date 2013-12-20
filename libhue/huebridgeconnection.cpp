#include "huebridgeconnection.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QSettings>
#include <QDebug>

HueBridgeConnection *HueBridgeConnection::s_instance = 0;

HueBridgeConnection *HueBridgeConnection::instance()
{
    if (!s_instance) {
        s_instance = new HueBridgeConnection();
    }
    return s_instance;
}

HueBridgeConnection::HueBridgeConnection():
    m_nam(new QNetworkAccessManager(this)),
    m_requestCounter(0)
{
    QSettings settings;
    if (settings.contains("username")) {
        m_username = settings.value("username").toString();
        emit usernameChanged();
    }

}

QString HueBridgeConnection::username() const
{
    return m_username;
}

void HueBridgeConnection::setUsername(const QString &username)
{
    if (username != m_username) {
        m_username = username;
        emit usernameChanged();
    }
}

void HueBridgeConnection::createUser(const QString &devicetype, const QString &username)
{
    QVariantMap params;
    params.insert("devicetype", devicetype);
    params.insert("username", username);
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);

    QNetworkRequest request;
    request.setUrl(QUrl("http://10.10.10.123/api"));
    QNetworkReply *reply = m_nam->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HueBridgeConnection::createUserFinished);
}


int HueBridgeConnection::get(const QString &path, QObject *sender, const QString &slot)
{
    if (m_username.isEmpty()) {
        qWarning() << "No username set. cannot proceed.";
        return -1;
    }
    QUrl url("http://10.10.10.123/api/" + m_username + "/" + path);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, &HueBridgeConnection::slotGetFinished);
    m_requestIdMap.insert(reply, m_requestCounter);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

int HueBridgeConnection::post(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot)
{
    if (m_username.isEmpty()) {
        qWarning() << "No username set, cannot proceed";
        return -1;
    }

    QUrl url("http://10.10.10.123/api/" + m_username + "/" + path);
    url.setAuthority(path);
    QNetworkRequest request;
    request.setUrl(url);

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);

    QNetworkReply *reply = m_nam->post(request, jsonDoc.toJson());
    connect(reply, &QNetworkReply::finished, this, &HueBridgeConnection::slotGetFinished);
    m_requestIdMap.insert(reply, m_requestCounter);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

void HueBridgeConnection::createUserFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    QByteArray response = reply->readAll();
    qDebug() << "create user finished" << response;
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "cannot parse response:" << error.errorString();
        return;
    }

    QVariant rsp = jsonDoc.toVariant();
    QVariantMap map = rsp.toList().first().toMap();
    if (map.contains("error")) {
        qWarning() << "error creating user" << map.value("error").toMap().value("description").toString();
        return;
    }

    if (!map.contains("success")) {
        qWarning() << "Huh? Neither error nor success contained in response...";
        return;
    }

    m_username = map.value("success").toMap().value("username").toString();
    emit usernameChanged();

    // TODO: move this away from the lib
    QSettings settings;
    settings.setValue("username", m_username);
}

void HueBridgeConnection::slotGetFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QByteArray response = reply->readAll();
    int id = m_requestIdMap.take(reply);
    CallbackObject co = m_requestSenderMap.take(id);

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    if (!error.error == QJsonParseError::NoError) {
        qWarning() << "error parsing response:" << error.errorString();
        return;
    }

    QMetaObject::invokeMethod(co.sender(), co.slot().toLatin1().data(), Q_ARG(int, id), Q_ARG(QVariant, jsonDoc.toVariant()));

}

