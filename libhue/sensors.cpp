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

Sensors::Sensors(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
    refresh();

    connect(&m_timer, &QTimer::timeout, this, &Sensors::refresh);
    m_timer.start(10000);

#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Sensors::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int Sensors::count() const
{
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
    }

    return QVariant();
}

QHash<int, QByteArray> Sensors::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
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

//void Schedules::createSingleAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &dateTime)
//{
//    createAlarmForScene(name, sceneId, dateTime.toString(Qt::ISODate));
//}

//void Schedules::createRecurringAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &time, const QString &weekdays)
//{
//    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
//    createAlarmForScene(name, sceneId, timeString);
//}

//void Schedules::createRecurringAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays)
//{
//    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
//    createScheduleForLight(name, lightId, on, bri, color, timeString);
//}

//void Schedules::createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime)
//{
//    createScheduleForGroup(name, groupId, on, bri, color, dateTime.toString(Qt::ISODate));
//}

//void Schedules::createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays)
//{
//    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
//    createScheduleForGroup(name, groupId, on, bri, color, timeString);
//}

//void Schedules::createTimerForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat)
//{
//    QString timeString;
//    if (repeat >= 0) {
//        timeString += "R";
//        if (repeat > 0) {
//            timeString += QString::number(repeat);
//        }
//        timeString += "/";
//    }
//    timeString += "PT" + timeFromNow.toString("hh:mm:ss");

//    createScheduleForLight(name, lightId, on, bri, color, timeString);
//}

//void Schedules::createTimerForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat)
//{
//    QString timeString;
//    if (repeat >= 0) {
//        timeString += "R";
//        if (repeat > 0) {
//            timeString += QString::number(repeat);
//        }
//        timeString += "/";
//    }
//    timeString += "PT" + timeFromNow.toString("hh:mm:ss");

//    createScheduleForGroup(name, groupId, on, bri, color, timeString);
//}

//void Schedules::createSingleAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime)
//{
//    createScheduleForLight(name, lightId, on, bri, color, dateTime.toString(Qt::ISODate));
//}

void Sensors::refresh()
{
    HueBridgeConnection::instance()->get("sensors", this, "sensorsReceived");
}

//void Schedules::createAlarmForScene(const QString &name, const QString &sceneId, const QString &timeString)
//{
//    QVariantMap commandParams;
//    commandParams.insert("scene", sceneId);

//    QVariantMap command;
//    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/groups/0/action");
//    command.insert("method", "PUT");
//    command.insert("body", commandParams);

//    createSchedule(name, command, timeString);
//}

//void Schedules::createScheduleForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QString &timeString)
//{
//    QVariantMap commandParams;
//    commandParams.insert("on", on);
//    commandParams.insert("bri", bri);
//    // Transform from RGB to Hue/Sat
//    commandParams.insert("hue", color.hue() * 65535 / 360);
//    commandParams.insert("sat", color.saturation());

//    QVariantMap command;
//    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/lights/" + QString::number(lightId) + "/state");
//    command.insert("method", "PUT");
//    command.insert("body", commandParams);

//    createSchedule(name, command, timeString);
//}

//void Schedules::createScheduleForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QString &timeString)
//{
//    QVariantMap commandParams;
//    commandParams.insert("on", on);
//    commandParams.insert("bri", bri);
//    // Transform from RGB to Hue/Sat
//    commandParams.insert("hue", color.hue() * 65535 / 360);
//    commandParams.insert("sat", color.saturation());

//    QVariantMap command;
//    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/groups/" + QString::number(groupId) + "/action");
//    command.insert("method", "PUT");
//    command.insert("body", commandParams);

//    createSchedule(name, command, timeString);
//}

//void Schedules::createSchedule(const QString &name, const QVariantMap &command, const QString &timeString)
//{
//    QVariantMap params;
//    params.insert("name", name);
//    params.insert("command", command);
//    params.insert("localtime", timeString);
//    HueBridgeConnection::instance()->post("schedules", params, this, "createScheduleFinished");
//}

void Sensors::sensorsReceived(int id, const QVariant &variant)
{
    qDebug() << "**** sensors received" << variant;
    Q_UNUSED(id)
    QVariantMap sensors = variant.toMap();
    QList<Sensor*> removedSensors;
    foreach (Sensor *sensor, m_list) {
        if (!sensors.contains(sensor->id())) {
            qDebug() << "removing sensor" << sensor->id();
            removedSensors.append(sensor);
        } else {
            qDebug() << "updating sensor" << sensor->id();
            QVariantMap sensorMap = sensors.value(sensor->id()).toMap();
            sensor->setName(sensorMap.value("name").toString());
        }
    }

    qDebug() << removedSensors.count() << "sensors removed";
    foreach (Sensor *sensor, removedSensors) {
        int index = m_list.indexOf(sensor);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &sensorId, sensors.keys()) {
        if (findSensor(sensorId) == 0) {
            QVariantMap sensorMap = sensors.value(sensorId).toMap();
            Sensor *sensor = createSensorInternal(sensorId, sensorMap.value("name").toString());
        }
    }
    emit countChanged();
}

//void Schedules::deleteSchedule(const QString &id)
//{
//    HueBridgeConnection::instance()->deleteResource("schedules/" + id, this, "deleteScheduleFinished");
//}

Sensor *Sensors::createSensorInternal(const QString &id, const QString &name)
{
    Sensor *sensor = new Sensor(id, name, this);

//    connect(sensor, SIGNAL(nameChanged()), this, SLOT(sceneNameChanged()));
//    connect(sensor, SIGNAL(activeChanged()), this, SLOT(sceneActiveChanged()));

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(sensor);
    endInsertRows();
    return sensor;
}

//void Schedules::createScheduleFinished(int id, const QVariant &response)
//{
//    Q_UNUSED(id)
//    qDebug() << "got createScene result" << response;

//    QVariantMap result = response.toList().first().toMap();

//    if (result.contains("success")) {
//        //TODO: could be added without refrshing, but we don't know the name at this point.
//        //TODO: might be best to ctor groups/lights with id only and make them fetch their own info.
//        refresh();
//    }
//}

//void Schedules::deleteScheduleFinished(int id, const QVariant &response)
//{
//    Q_UNUSED(id)
//    qDebug() << "got deleteSchedule result" << response;

//    QVariantMap result = response.toList().first().toMap();

//    if (result.contains("success")) {
//        //TODO: could be deleted without refrshing
//        refresh();
//    }
//}
