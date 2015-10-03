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

#include "schedules.h"
#include "schedule.h"

#include "huebridgeconnection.h"

#include <QDebug>
#include <QUuid>
#include <QColor>

Schedules::Schedules(QObject *parent):
    HueModel(parent),
    m_busy(false)
{

#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Schedules::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Schedules::data(const QModelIndex &index, int role) const
{
    Schedule *schedule = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return schedule->id();
    case RoleName:
        return schedule->name();
    case RoleDateTime:
        return schedule->dateTime();
    case RoleType:
        return schedule->type();
    case RoleRecurring:
        return schedule->recurring();
    case RoleWeekdays:
        return schedule->weekdays();
    }

    return QVariant();
}

QHash<int, QByteArray> Schedules::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleDateTime, "dateTime");
    roles.insert(RoleType, "type");
    roles.insert(RoleRecurring, "recurring");
    roles.insert(RoleWeekdays, "weekdays");
    return roles;
}

Schedule *Schedules::get(int index) const
{
    if (index > -1 && index  < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Schedule *Schedules::findSchedule(const QString &id) const
{
    foreach (Schedule *scene, m_list) {
        if (scene->id() == id) {
            return scene;
        }
    }
    return 0;
}

bool Schedules::busy() const
{
    return m_busy;
}

void Schedules::createSingleAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &dateTime)
{
    createAlarmForScene(name, sceneId, dateTime.toString(Qt::ISODate));
}

void Schedules::createRecurringAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &time, const QString &weekdays)
{
    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
    createAlarmForScene(name, sceneId, timeString);
}

void Schedules::createRecurringAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays)
{
    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
    createScheduleForLight(name, lightId, on, bri, color, timeString);
}

void Schedules::createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime)
{
    createScheduleForGroup(name, groupId, on, bri, color, dateTime.toString(Qt::ISODate));
}

void Schedules::createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays)
{
    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
    createScheduleForGroup(name, groupId, on, bri, color, timeString);
}

void Schedules::createTimerForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat)
{
    QString timeString;
    if (repeat >= 0) {
        timeString += "R";
        if (repeat > 0) {
            timeString += QString::number(repeat);
        }
        timeString += "/";
    }
    timeString += "PT" + timeFromNow.toString("hh:mm:ss");

    createScheduleForLight(name, lightId, on, bri, color, timeString);
}

void Schedules::createTimerForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat)
{
    QString timeString;
    if (repeat >= 0) {
        timeString += "R";
        if (repeat > 0) {
            timeString += QString::number(repeat);
        }
        timeString += "/";
    }
    timeString += "PT" + timeFromNow.toString("hh:mm:ss");

    createScheduleForGroup(name, groupId, on, bri, color, timeString);
}

void Schedules::createSingleAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime)
{
    createScheduleForLight(name, lightId, on, bri, color, dateTime.toString(Qt::ISODate));
}

void Schedules::refresh()
{
    HueBridgeConnection::instance()->get("schedules", this, "schedulesReceived");
    m_busy = true;
    emit busyChanged();
}

void Schedules::createAlarmForScene(const QString &name, const QString &sceneId, const QString &timeString)
{
    QVariantMap commandParams;
    commandParams.insert("scene", sceneId);

    QVariantMap command;
    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/groups/0/action");
    command.insert("method", "PUT");
    command.insert("body", commandParams);

    createSchedule(name, command, timeString);
}

void Schedules::createScheduleForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QString &timeString)
{
    QVariantMap commandParams;
    commandParams.insert("on", on);
    commandParams.insert("bri", bri);
    // Transform from RGB to Hue/Sat
    commandParams.insert("hue", color.hue() * 65535 / 360);
    commandParams.insert("sat", color.saturation());

    QVariantMap command;
    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/lights/" + QString::number(lightId) + "/state");
    command.insert("method", "PUT");
    command.insert("body", commandParams);

    createSchedule(name, command, timeString);
}

void Schedules::createScheduleForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QString &timeString)
{
    QVariantMap commandParams;
    commandParams.insert("on", on);
    commandParams.insert("bri", bri);
    // Transform from RGB to Hue/Sat
    commandParams.insert("hue", color.hue() * 65535 / 360);
    commandParams.insert("sat", color.saturation());

    QVariantMap command;
    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/groups/" + QString::number(groupId) + "/action");
    command.insert("method", "PUT");
    command.insert("body", commandParams);

    createSchedule(name, command, timeString);
}

void Schedules::createSchedule(const QString &name, const QVariantMap &command, const QString &timeString)
{
    QVariantMap params;
    params.insert("name", name);
    params.insert("command", command);
    params.insert("localtime", timeString);
    HueBridgeConnection::instance()->post("schedules", params, this, "createScheduleFinished");
}

void Schedules::schedulesReceived(int id, const QVariant &variant)
{
//    qDebug() << "**** schedules received" << variant;
    Q_UNUSED(id)
    QVariantMap schedules = variant.toMap();
    QList<Schedule*> removedSchedules;
    foreach (Schedule *schedule, m_list) {
        if (!schedules.contains(schedule->id())) {
//            qDebug() << "removing schedule" << schedule->id();
            removedSchedules.append(schedule);
        } else {
//            qDebug() << "updating schedule" << schedule->id();
            QVariantMap scheduleMap = schedules.value(schedule->id()).toMap();
            schedule->setName(scheduleMap.value("name").toString());
            schedule->setEnabled(scheduleMap.value("status").toString() == "enabled");
            schedule->setAutoDelete(scheduleMap.value("autodelete").toBool());
            schedule->setDateTime(scheduleMap.value("time").toDateTime());
        }
    }

//    qDebug() << removedSchedules.count() << "schedules removed";
    foreach (Schedule *schedule, removedSchedules) {
        int index = m_list.indexOf(schedule);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &scheduleId, schedules.keys()) {
        if (findSchedule(scheduleId) == 0) {
            QVariantMap scheduleMap = schedules.value(scheduleId).toMap();
            Schedule *schedule = createScheduleInternal(scheduleId, scheduleMap.value("name").toString());
            schedule->setEnabled(scheduleMap.value("status").toString() == "enabled");
            schedule->setAutoDelete(scheduleMap.value("autodelete").toBool());
            QString timeString = scheduleMap.value("localtime").toString();
            if (timeString.startsWith("W")) {
                schedule->setRecurring(true);
                timeString = timeString.right(timeString.length() - 1);
                schedule->setWeekdays(QString("0%1").arg(timeString.left(3).toInt(), 7, 2, QChar('0')));
                timeString = timeString.right(timeString.length() - 5);
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(0);
                dateTime.setTime(QTime::fromString(timeString));
                schedule->setDateTime(dateTime);
            } else if (timeString.contains("PT")){
                schedule->setType(Schedule::TypeTimer);
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(0);
                dateTime.setTime(QTime::fromString(timeString.remove("PT")));
                schedule->setDateTime(dateTime);
            } else {
                schedule->setDateTime(scheduleMap.value("localtime").toDateTime());
            }
        }
    }
    emit countChanged();

    m_busy = false;
    emit busyChanged();
}

void Schedules::deleteSchedule(const QString &id)
{
    HueBridgeConnection::instance()->deleteResource("schedules/" + id, this, "deleteScheduleFinished");
}

Schedule *Schedules::createScheduleInternal(const QString &id, const QString &name)
{
    Schedule *schedule = new Schedule(id, name, this);

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(schedule);
    endInsertRows();
    return schedule;
}

void Schedules::createScheduleFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "got createScene result" << response;

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        //TODO: could be added without refrshing, but we don't know the name at this point.
        //TODO: might be best to ctor groups/lights with id only and make them fetch their own info.
        refresh();
    }
}

void Schedules::deleteScheduleFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "got deleteSchedule result" << response;

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        //TODO: could be deleted without refrshing
        refresh();
    }
}
