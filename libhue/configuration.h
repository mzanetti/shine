/*
 * Copyright 2013 Michael Zanetti
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
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "hueobject.h"

#include <QVariantMap>
#include <QTimer>

class Configuration: public HueObject
{
    Q_OBJECT

    Q_ENUMS(UpdateState)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY changed)
    Q_PROPERTY(bool connectedToPortal READ connectedToPortal NOTIFY changed)
    Q_PROPERTY(QString swVersion READ swVersion NOTIFY changed)
    Q_PROPERTY(UpdateState updateState READ updateState NOTIFY changed)
    Q_PROPERTY(QString swUpdateReleaseNotes READ swUpdateReleaseNotes NOTIFY changed)

public:
    enum UpdateState {
        UpdateStateUpToDate = 0,
        UpdateStateDownloading = 1,
        UpdateStateReadyToUpdate = 2,
        UpdateStateUpdating = 3
    };

    Configuration(QObject *parent = 0);

    QString name();
    void setName(const QString &name);

    bool connectedToPortal() const;
    QString swVersion() const;
    UpdateState updateState() const;
    QString swUpdateReleaseNotes() const;

public slots:
    void refresh();

    void checkForUpdate();
    void performUpdate();

signals:
    void changed();

private slots:
    void responseReceived(int id, const QVariant &data);
    void checkForUpdateReply(int id, const QVariant &data);
    void performUpdateReply(int id, const QVariant &data);

private:
    QString m_name;
    bool m_connectedToPortal;
    QString m_swVersion;
    UpdateState m_updateState;
    QString m_url;
};

#endif
