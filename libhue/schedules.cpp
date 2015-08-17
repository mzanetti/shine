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

Schedules::Schedules(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
    refresh();

#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Schedules::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

int Schedules::count() const
{
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

void Schedules::createSingleAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &dateTime)
{
    createAlarmForScene(name, sceneId, dateTime.toString(Qt::ISODate));
}

void Schedules::createRecurringAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &time, const QString &weekdays)
{
    QString timeString = "W" + QString::number(weekdays.toInt(0, 2)) + "/T" + time.time().toString();
    createAlarmForScene(name, sceneId, timeString);
}

//void Scenes::recallScene(int index)
//{
//    QVariantMap params;
//    params.insert("scene", m_list.at(index)->id());
//    HueBridgeConnection::instance()->put("groups/0/action", params, this, "recallSceneFinished");
//}

void Schedules::refresh()
{
    HueBridgeConnection::instance()->get("schedules", this, "schedulesReceived");
}

void Schedules::createAlarmForScene(const QString &name, const QString &sceneId, const QString &timeString)
{
    QVariantMap commandParams;
    commandParams.insert("scene", sceneId);

    QVariantMap command;
    command.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/groups/0/action");
    command.insert("method", "PUT");
    command.insert("body", commandParams);

    QVariantMap params;
    params.insert("name", name);
    params.insert("command", command);
    params.insert("localtime", timeString);
    HueBridgeConnection::instance()->post("schedules", params, this, "createScheduleFinished");
}

void Schedules::schedulesReceived(int id, const QVariant &variant)
{
    qDebug() << "**** schedules received" << variant;
    Q_UNUSED(id)
    QVariantMap schedules = variant.toMap();
    QList<Schedule*> removedSchedules;
    foreach (Schedule *schedule, m_list) {
        if (!schedules.contains(schedule->id())) {
            qDebug() << "removing schedule" << schedule->id();
            removedSchedules.append(schedule);
        } else {
            qDebug() << "updating schedule" << schedule->id();
            QVariantMap scheduleMap = schedules.value(schedule->id()).toMap();
            schedule->setName(scheduleMap.value("name").toString());
            schedule->setEnabled(scheduleMap.value("status").toString() == "enabled");
            schedule->setAutoDelete(scheduleMap.value("autodelete").toBool());
            schedule->setDateTime(scheduleMap.value("time").toDateTime());
        }
    }

    qDebug() << removedSchedules.count() << "schedules removed";
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
            QString timeString = scheduleMap.value("time").toString();
            if (timeString.startsWith("W")) {
                schedule->setRecurring(true);
                timeString = timeString.right(timeString.length() - 1);
                schedule->setWeekdays(QString("0%1").arg(timeString.left(3).toInt(), 7, 2, QChar('0')));
                qDebug() << "fooo" << schedule->weekdays();
                timeString = timeString.right(timeString.length() - 5);
                QDateTime dateTime = QDateTime::fromMSecsSinceEpoch(0);
                dateTime.setTime(QTime::fromString(timeString));
                schedule->setDateTime(dateTime);
            } else {
                schedule->setDateTime(scheduleMap.value("time").toDateTime());
            }
            qDebug() << "timeString" << timeString << QDateTime::fromString(timeString) << schedule->weekdays();
        }
    }
    emit countChanged();
}

//void Schedules::sceneNameChanged()
//{
//    Scene *scene = static_cast<Scene*>(sender());
//    int idx = m_list.indexOf(scene);
//    QModelIndex modelIndex = index(idx);

//#if QT_VERSION >= 0x050000
//    QVector<int> roles = QVector<int>()
//            << RoleName;

//    emit dataChanged(modelIndex, modelIndex, roles);
//#else
//    emit dataChanged(modelIndex, modelIndex);
//#endif
//}

//void Groups::groupLightsChanged()
//{
//    Group *group = static_cast<Group*>(sender());
//    int idx = m_list.indexOf(group);
//    QModelIndex modelIndex = index(idx);

//#if QT_VERSION >= 0x050000
//    QVector<int> roles = QVector<int>() << RoleLightIds;
//    emit dataChanged(modelIndex, modelIndex, roles);
//#else
//    emit dataChanged(modelIndex, modelIndex);
//#endif
//}

void Schedules::createSchedule(const QString &name, const QList<int> &lights)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i < 12; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    updateSchedule("schedule" + randomString, name, lights);
}

void Schedules::updateSchedule(const QString &id, const QString &name, const QList<int> &lights)
{
    qDebug() << "create schedule" << name << lights;
    QVariantMap params;
    QVariantList lightsList;
    foreach (int lightId, lights) {
        qDebug() << "got light" << lightId;
        lightsList.append(QString::number(lightId));
    }
    qDebug() << "lightslist" << lightsList;
    params.insert("name", name);
    params.insert("lights", lightsList);
    HueBridgeConnection::instance()->put("scenes/" + id, params, this, "createSceneFinished");
}

void Schedules::deleteSchedule(const QString &id)
{
    HueBridgeConnection::instance()->deleteResource("schedules/" + id, this, "deleteScheduleFinished");
}

Schedule *Schedules::createScheduleInternal(const QString &id, const QString &name)
{
    Schedule *schedule = new Schedule(id, name, this);

    connect(schedule, SIGNAL(nameChanged()), this, SLOT(sceneNameChanged()));
    connect(schedule, SIGNAL(activeChanged()), this, SLOT(sceneActiveChanged()));

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

//void Scenes::recallSceneFinished(int id, const QVariant &variant)
//{
//    qDebug() << "scene recalled" << variant;
//}

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
