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
#include <QDebug>
#if QT_VERSION >= 0x050000
#include <QJsonDocument>
#else
#include "qjson/parser.h"
#include "qjson/serializer.h"
#endif

#include "discovery.h"

HueBridgeConnection *HueBridgeConnection::s_instance = NULL;

HueBridgeConnection *HueBridgeConnection::instance()
{
    if (s_instance == NULL) {
        s_instance = new HueBridgeConnection();
    }
    return s_instance;
}

QString HueBridgeConnection::apiKey() const
{
    return m_apiKey;
}

void HueBridgeConnection::setApiKey(const QString &apiKey)
{
    if (m_apiKey != apiKey) {
        m_apiKey = apiKey;
        emit apiKeyChanged();
    }
}

bool HueBridgeConnection::discoveryError() const
{
    return m_discoveryError;
}

bool HueBridgeConnection::bridgeFound() const
{
    return !m_bridge.isNull();
}

QString HueBridgeConnection::connectedBridge() const
{
    return m_apiKey.isEmpty() ? "" : m_bridge.toString();
}

HueBridgeConnection::BridgeStatus HueBridgeConnection::status() const
{
    return m_bridgeStatus;
}

HueBridgeConnection::HueBridgeConnection():
    m_nam(new QNetworkAccessManager(this)),
    m_discoveryError(false),
    m_bridgeStatus(BridgeStatusSearching),
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
    qDebug() << Q_FUNC_INFO << "Error discovering hue bridge!";
    m_discoveryError = true;
    emit discoveryErrorChanged();
}

void HueBridgeConnection::onFoundBridge(QHostAddress bridge)
{
    //FIXME: eventually handle multiple bridges
    disconnect(sender());
    m_bridge = bridge;

    if (!m_apiKey.isEmpty()) {
        m_baseApiUrl = "http://" + m_bridge.toString() + "/api/" + m_apiKey + "/";
    }

    // Emitting this after we know if we can connect or not to avoid the ui triggering connect dialogs
    m_bridgeStatus = BridgeStatusConnecting;
    emit statusChanged();
    emit bridgeFoundChanged();

    // Tell the bridge to check for firmware updates
    QVariantMap swupdateMap;
    swupdateMap.insert("checkforupdate", true);
    QVariantMap params;
    params.insert("portalservices", true);
    params.insert("swupdate", swupdateMap);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    QNetworkRequest request(m_baseApiUrl + "config");
    QNetworkReply *reply = m_nam->put(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(checkForUpdateFinished()));
}

void HueBridgeConnection::onNoBridgesFound()
{
    //FIXME: handle error case
    qDebug() << Q_FUNC_INFO << "No hue bridges found!";
}

void HueBridgeConnection::createUser(const QString &devicetype)
{
    QVariantMap params;
    params.insert("devicetype", devicetype);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    qDebug() << "sending createUser to" << m_bridge.toString();
    QNetworkRequest request;
    request.setUrl(QUrl("http://" + m_bridge.toString() + "/api"));
    QNetworkReply *reply = m_nam->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(createUserFinished()));
}

int HueBridgeConnection::get(const QString &path, QObject *sender, const QString &slot)
{
    if (m_baseApiUrl.isEmpty()) {
        qWarning() << "Not authenticated to bridge, cannot get" << path;
        return -1;
    }
    QUrl url(m_baseApiUrl + path);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_nam->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotOpFinished()));
    m_requestIdMap.insert(reply, m_requestCounter);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

int HueBridgeConnection::deleteResource(const QString &path, QObject *sender, const QString &slot)
{
    if (m_baseApiUrl.isEmpty()) {
        qWarning() << "Not authenticated to bridge, cannot delete" << path;
        return -1;
    }
    QUrl url(m_baseApiUrl + path);
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = m_nam->deleteResource(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotOpFinished()));
    m_requestIdMap.insert(reply, m_requestCounter);
    m_writeOperationList.append(reply);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

int HueBridgeConnection::post(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot)
{
    if (m_baseApiUrl.isEmpty()) {
        qWarning() << "Not authenticated to bridge, cannot post" << path;
        return -1;
    }

    QUrl url(m_baseApiUrl + path);
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(url);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson(QJsonDocument::Compact);
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif

    qDebug() << "posting" << jsonDoc.toJson()<< "\nto" << request.url() << "\n" << data;
    QNetworkReply *reply = m_nam->post(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(slotOpFinished()));
    m_requestIdMap.insert(reply, m_requestCounter);
    m_writeOperationList.append(reply);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

int HueBridgeConnection::put(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot)
{
    if (m_baseApiUrl.isEmpty()) {
        qWarning() << "Not authenticated to bridge, cannot put" << path;
        return -1;
    }

    QUrl url(m_baseApiUrl + path);
    QNetworkRequest request;
    request.setUrl(url);

#if QT_VERSION >= 0x050000
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(params);
    QByteArray data = jsonDoc.toJson();
#else
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(params);
#endif
//    qDebug() << "putting" << url << data;

    QNetworkReply *reply = m_nam->put(request, data);
    connect(reply, SIGNAL(finished()), this, SLOT(slotOpFinished()));
    m_requestIdMap.insert(reply, m_requestCounter);
    m_writeOperationList.append(reply);
    CallbackObject co(sender, slot);
    m_requestSenderMap.insert(m_requestCounter, co);
    return m_requestCounter++;
}

void HueBridgeConnection::createUserFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QByteArray response = reply->readAll();
    qDebug() << "create user finished" << response;

#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "cannot parse response:" << error.errorString() << response;
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

    setApiKey(map.value("success").toMap().value("username").toString());

    m_baseApiUrl = "http://" + m_bridge.toString() + "/api/" + m_apiKey + "/";
    emit connectedBridgeChanged();
}

void HueBridgeConnection::checkForUpdateFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QByteArray response = reply->readAll();
    qDebug() << "check for update finished" << response;

#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);

    if (error.error != QJsonParseError::NoError) {
        qWarning() << "cannot parse response:" << error.errorString() << response;
        emit statusChanged();
        return;
    }
    QVariant rsp = jsonDoc.toVariant();
#else
    QJson::Parser parser;
    bool ok;
    QVariant rsp = parser.parse(response, &ok);
    if(!ok) {
        qWarning() << "cannot parse response:" << response;
        emit statusChanged();
        return;
    }
#endif

    if (rsp.toList().first().toMap().contains("error")) {
        if (rsp.toList().first().toMap().value("error").toMap().value("type").toInt() == 1) {
            qWarning() << "User not authenticated to bridge";
            m_bridgeStatus = BridgeStatusAuthenticationFailure;
        } else {
            qDebug() << "error communicating with bridge:" << jsonDoc.toJson();
        }
    } else {
        m_bridgeStatus = BridgeStatusConnected;
        emit connectedBridgeChanged();
    }
    emit statusChanged();
}

void HueBridgeConnection::slotOpFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QByteArray response = reply->readAll();
    int id = m_requestIdMap.take(reply);
    CallbackObject co = m_requestSenderMap.take(id);

    qDebug() << "reply for" << co.sender() << co.slot();
//    qDebug() << "response" << response;

    QVariant rsp;
#if QT_VERSION >= 0x050000
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(response, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "error parsing get response:" << error.errorString() << response;
    } else {
        rsp = jsonDoc.toVariant();
    }
#else
    QJson::Parser parser;
    bool ok;
    rsp = parser.parse(response, &ok);
    if(!ok) {
        qWarning() << "cannot parse response:" << response;
    }
#endif

    if (m_writeOperationList.contains(reply)) {
        m_writeOperationList.removeAll(reply);
    }

    if (!co.sender().isNull()) {
        QMetaObject::invokeMethod(co.sender(), co.slot().toLatin1().data(), Q_ARG(int, id), Q_ARG(QVariant, rsp));
    }
}

