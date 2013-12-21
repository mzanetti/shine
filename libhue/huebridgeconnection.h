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

#ifndef HUEBRIDGECONNECTION_H
#define HUEBRIDGECONNECTION_H

#include <QObject>
#include <QHash>
#include <QHostAddress>
#include <QVariantMap>

class QNetworkAccessManager;
class QNetworkReply;

class CallbackObject
{
public:
    CallbackObject(QObject *sender = 0, const QString &slot = QString()):
        m_sender(sender),
        m_slot(slot)
    {}
    QObject *sender() const { return m_sender; }
    QString slot() const { return m_slot; }
private:
    QObject *m_sender;
    QString m_slot;
};

class HueBridgeConnection: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)

public:
    static HueBridgeConnection* instance();

    QString username() const;
    void setUsername(const QString &username);
    Q_INVOKABLE void createUser(const QString &devicetype, const QString &username);

    int get(const QString &path, QObject *sender, const QString &slot);
    int post(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot);
    int put(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot);

signals:
    void usernameChanged();
    void createUserFailed(const QString &errorMessage);

private slots:
    void onDiscoveryError();
    void onFoundBridge(QHostAddress bridge);
    void onNoBridgesFound();

    void createUserFinished();
    void slotGetFinished();
    void slotPutFinished();

private:
    HueBridgeConnection();
    static HueBridgeConnection *s_instance;

    QNetworkAccessManager *m_nam;
    QString m_username;

    int m_requestCounter;
    QHash<QNetworkReply*, int> m_requestIdMap;
    QHash<int, CallbackObject> m_requestSenderMap;

    QHostAddress m_bridge;
};

#endif
