/*
 * Copyright 2013 Michael Zanetti
 *                Christian Muehlhaeuser
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Michael Zanetti <michael_zanetti@gmx.net>
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "huebridgeconnection.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QSettings>
#include <QDebug>
#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#else
#include "qjson/parser.h"
#include "qjson/serializer.h"
#endif

#include "discovery.h"

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
    Discovery *discovery = new Discovery(this);
    connect(discovery, SIGNAL(error()), this, SLOT(onDiscoveryError()));
    connect(discovery, SIGNAL(foundBridge(QHostAddress)), this, SLOT(onFoundBridge(QHostAddress)));
    connect(discovery, SIGNAL(noBridgesFound()), this, SLOT(onNoBridgesFound()));
    discovery->findBridges();
}

void HueBridgeConnection::onDiscoveryError()
{
    //FIXME: handle error case
    qDebug() << Q_FUNC_INFO << "Error discovering hue bridge!";
}

void HueBridgeConnection::onFoundBridge(QHostAddress bridge)
{
    //FIXME: eventually handle multiple bridges
    disconnect(sender());

    m_bridge = bridge;

    QSettings settings;
    if (settings.contains("username")) {
        m_username = settings.value("username").toString();
        emit usernameChanged();
    }
}

void HueBridgeConnection::onNoBridgesFound()
{
    //FIXME: handle error case
    qDebug() << Q_FUNC_INFO << "No hue bridges found!";
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

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_bridge.toString() + "/api"));
    QNetworkReply *reply = m_nam->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(createUserFinished()));
}


int HueBridgeConnection::get(const QString &path, QObject *sender, const QString &slot)
{
    if (m_username.isEmpty()) {
        qWarning() << "No username set. cannot proceed.";
        return -1;
    }
    QUrl url("http://" + m_bridge.toString() + "/api/" + m_username + "/" + path);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_nam->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotGetFinished()));
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

    QUrl url("http://" + m_bridge.toString() + "/api/" + m_username + "/" + path);
    QNetworkRequest request;
    request.setUrl(url);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    QNetworkReply *reply = m_nam->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(slotGetFinished()));
    m_requestIdMap.insert(reply, m_requestCounter);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

int HueBridgeConnection::put(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot)
{
    if (m_username.isEmpty()) {
        qWarning() << "No username set, cannot proceed";
        return -1;
    }

    QUrl url("http://" + m_bridge.toString() + "/api/" + m_username + "/" + path);
    QNetworkRequest request;
    request.setUrl(url);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    QNetworkReply *reply = m_nam->put(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(slotPutFinished()));
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

#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "cannot parse response:" << error.errorString();
        return;
    }
    QVariant rsp = jsonDoc.toVariant();
#else
    QJson::Parser parser;
    bool ok;
    QVariant rsp = parser.parse(response, &ok);
    if(!ok) {
        qWarning() << "cannot parse response:" << response;
        return;
    }
    qDebug() << "json parsed" << rsp;
#endif

    QVariantMap map = rsp.toList().first().toMap();
    if (map.contains("error")) {
        QString errorMessage = map.value("error").toMap().value("description").toString();
        qWarning() << "error creating user" << errorMessage;
        emit createUserFailed(errorMessage);
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


#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "error parsing get response:" << error.errorString();
        return;
    }
    QVariant rsp = jsonDoc.toVariant();
#else
    QJson::Parser parser;
    bool ok;
    QVariant rsp = parser.parse(response, &ok);
    if(!ok) {
        qWarning() << "cannot parse response:" << response;
        return;
    }
#endif

    QMetaObject::invokeMethod(co.sender(), co.slot().toLatin1().data(), Q_ARG(int, id), Q_ARG(QVariant, rsp));

}

void HueBridgeConnection::slotPutFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QByteArray response = reply->readAll();
    int id = m_requestIdMap.take(reply);
    CallbackObject co = m_requestSenderMap.take(id);

#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "error parsing put response:" << error.errorString();
        return;
    }
    QVariant rsp = jsonDoc.toVariant();
#else
    QJson::Parser parser;
    bool ok;
    QVariant rsp = parser.parse(response, &ok);
    if(!ok) {
        qWarning() << "cannot parse response:" << response;
        return;
    }
#endif

    QMetaObject::invokeMethod(co.sender(), co.slot().toLatin1().data(), Q_ARG(int, id), Q_ARG(QVariant, rsp));

}

