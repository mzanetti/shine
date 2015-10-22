/*
 * Copyright 2015 Michael Zanetti
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

#ifndef SENSORS_H
#define SENSORS_H

#include "huemodel.h"

#include <QTimer>

class Sensor;

class Sensors: public HueModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId = Qt::UserRole,
        RoleName,
        RoleType,
        RoleTypeString,
        RoleModelId,
        RoleManufacturerName,
        RoleUniqueId,
        RoleStateMap
    };

    explicit Sensors(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Sensor* get(int index) const;
    Q_INVOKABLE Sensor* findSensor(const QString &id) const;
    Q_INVOKABLE void createSensor(const QString &name, const QString &uniqueId);
    void deleteSensor(const QString &id);

    Q_INVOKABLE Sensor* findHelperSensor(const QString &name, const QString &uniqueId);
    Q_INVOKABLE Sensor* findOrCreateHelperSensor(const QString &name, const QString &uniqueId);

    bool busy() const;

public slots:
    void refresh();
    void sensorDeleted(int, const QVariant &response);

private slots:
    void sensorsReceived(int id, const QVariant &variant);
    void sensorCreated(int id, const QVariant &response);

private:
    QList<Sensor*> m_list;
    bool m_busy;
};

#endif // SCENES_H
