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

#include "sensors.h"
#include "sensor.h"

#include "huebridgeconnection.h"

#include <QDebug>
#include <QUuid>
#include <QColor>
#include <QCoreApplication>

Sensors::Sensors(QObject *parent):
    HueModel(parent),
    m_busy(false)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Sensors::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Sensors::data(const QModelIndex &index, int role) const
{
    Sensor *sensor = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return sensor->id();
    case RoleName:
        return sensor->name();
    case RoleType:
        return sensor->type();
    case RoleTypeString:
        return sensor->typeString();
    case RoleManufacturerName:
        return sensor->manufacturerName();
    case RoleUniqueId:
        return sensor->uniqueId();
    case RoleStateMap:
        return sensor->stateMap();
    case RoleModelId:
        return sensor->modelId();
    }

    return QVariant();
}

QHash<int, QByteArray> Sensors::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleType, "type");
    roles.insert(RoleTypeString, "typeString");
    roles.insert(RoleModelId, "modelId");
    roles.insert(RoleManufacturerName, "manufacturerName");
    roles.insert(RoleUniqueId, "uniqueId");
    roles.insert(RoleTypeString, "stateMap");
    return roles;
}

Sensor *Sensors::get(int index) const
{
    if (index > -1 && index  < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Sensor *Sensors::findSensor(const QString &id) const
{
    foreach (Sensor *sensor, m_list) {
        if (sensor->id() == id) {
            return sensor;
        }
    }
    return 0;
}

void Sensors::createSensor(const QString &name, const QString &uniqueId)
{
    QVariantMap params;
    params.insert("name", name);
    params.insert("type", Sensor::typeToString(Sensor::TypeClipGenericStatus));
    params.insert("modelid", "shine-helper-status");
    params.insert("manufacturername", "shine-ubuntu");
    params.insert("swversion", "0.1");
    params.insert("uniqueid", uniqueId);
    QVariantMap stateMap;
    stateMap.insert("status", 0);
    params.insert("state", stateMap);
    HueBridgeConnection::instance()->post("sensors", params, this, "sensorCreated");
}

Sensor *Sensors::findHelperSensor(const QString &name, const QString &uniqueId)
{
    foreach (Sensor *sensor, m_list) {
        if (sensor->name() == name && sensor->uniqueId() == uniqueId) {
            return sensor;
        }
    }
    return 0;
}

Sensor *Sensors::findOrCreateHelperSensor(const QString &name, const QString &uniqueId)
{
    Sensor *sensor = findHelperSensor(name, uniqueId);
    if (!sensor) {
        createSensor(name, uniqueId);
    }
    QDateTime endTime = QDateTime::currentDateTime().addSecs(10);
    while (endTime > QDateTime::currentDateTime()) {
        sensor = findHelperSensor(name, uniqueId);
        if (sensor) {
            return sensor;
        }
        qApp->processEvents();
    }
    return sensor;
}

bool Sensors::busy() const
{
    return m_busy;
}

void Sensors::refresh()
{
    HueBridgeConnection::instance()->get("sensors", this, "sensorsReceived");
    m_busy = true;
    emit busyChanged();
}

void Sensors::sensorsReceived(int id, const QVariant &variant)
{
//    qDebug() << "**** sensors received" << variant;
    Q_UNUSED(id)
    QVariantMap sensors = variant.toMap();
    QList<Sensor*> removedSensors;
    foreach (Sensor *sensor, m_list) {
        if (!sensors.contains(sensor->id())) {
            removedSensors.append(sensor);
        } else {
            QVariantMap sensorMap = sensors.value(sensor->id()).toMap();
            sensor->setName(sensorMap.value("name").toString());
        }
    }

    foreach (Sensor *sensor, removedSensors) {
        int index = m_list.indexOf(sensor);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &sensorId, sensors.keys()) {
        Sensor *sensor = findSensor(sensorId);
        QVariantMap sensorMap = sensors.value(sensorId).toMap();
        if (!sensor) {
            sensor = new Sensor(sensorId, sensorMap.value("name").toString(), this);
            sensor->setType(Sensor::typeStringToType(sensorMap.value("type").toString()));
            sensor->setStateMap(sensorMap.value("state").toMap());
            sensor->setModelId(sensorMap.value("modelid").toString());
            sensor->setManufacturerName(sensorMap.value("manufacturername").toString());
            sensor->setUniqueId(sensorMap.value("uniqueid").toString());

            beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
            m_list.append(sensor);
            endInsertRows();
        }
    }
    m_busy = true;
    emit busyChanged();
}

void Sensors::sensorCreated(int id, const QVariant &response)
{
    qDebug() << "sensor created" << response;
}
