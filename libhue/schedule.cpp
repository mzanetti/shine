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

#include "schedule.h"
#include "huebridgeconnection.h"

#include <QColor>
#include <QDebug>
#include <qabstractitemmodel.h>

Schedule::Schedule(const QString &id, const QString &name, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_type(TypeAlarm)
    , m_enabled(true)
    , m_autodelete(true)
    , m_recurring(false)
{
//    refresh();
}

QString Schedule::id() const
{
    return m_id;
}

QString Schedule::name() const
{
    return m_name;
}

void Schedule::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

Schedule::Type Schedule::type() const
{
    return m_type;
}

void Schedule::setType(Schedule::Type type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QDateTime Schedule::dateTime() const
{
    return m_dateTime;
}

void Schedule::setDateTime(const QDateTime &dateTime)
{
    if (m_dateTime != dateTime) {
        m_dateTime = dateTime;
        emit dateTimeChanged();
    }
}

QString Schedule::weekdays() const
{
    return m_weekdays;
}

void Schedule::setWeekdays(const QString &weekdays)
{
    if (m_weekdays != weekdays) {
        m_weekdays = weekdays;
        emit weekdaysChanged();
    }
}

bool Schedule::enabled() const
{
    return m_enabled;
}

void Schedule::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        emit enabledChanged();
    }
}

bool Schedule::autodelete() const
{
    return m_autodelete;
}

void Schedule::setAutoDelete(bool autodelete)
{
    if (m_autodelete != autodelete) {
        m_autodelete = autodelete;
        emit autodeleteChanged();
    }
}

bool Schedule::recurring() const
{
    return m_recurring;
}

void Schedule::setRecurring(bool recurring)
{
    if (m_recurring != recurring) {
        m_recurring = recurring;
        emit recurringChanged();
    }
}

void Schedule::refresh()
{
//    HueBridgeConnection::instance()->get("groups/" + QString::number(m_id), this, "responseReceived");
}

//void Scene::responseReceived(int id, const QVariant &response)
//{
//    Q_UNUSED(id)

//    m_lightIds.clear();

//    QVariantMap attributes = response.toMap();
//    QVariantList lightsMap = attributes.value("lights").toList();
//    foreach (const QVariant &lightId, lightsMap) {
//        m_lightIds << lightId.toUInt();
//    }

//    emit lightsChanged();

//    QVariantMap action = attributes.value("action").toMap();
//    m_on = action.value("on").toBool();
//    emit stateChanged();
//}

//void Group::setDescriptionFinished(int id, const QVariant &response)
//{
//    Q_UNUSED(id)
//    qDebug() << "setDescription finished" << response;
//    QVariantMap result = response.toList().first().toMap();

//    if (result.contains("success")) {
//        QVariantMap successMap = result.value("success").toMap();
//        if (successMap.contains("/groups/" + QString::number(m_id) + "/name")) {
//            m_name = successMap.value("/groups/" + QString::number(m_id) + "/name").toString();
//            emit nameChanged();
//        }
//    }
//}

//void Group::setStateFinished(int id, const QVariant &response)
//{
//    foreach (const QVariant &resultVariant, response.toList()) {
//        QVariantMap result = resultVariant.toMap();
//        if (result.contains("success")) {
//            QVariantMap successMap = result.value("success").toMap();
//            if (successMap.contains("/groups/" + QString::number(m_id) + "/state/on")) {
//                m_on = successMap.value("/groups/" + QString::number(m_id) + "/state/on").toBool();
//            }
//        }
//    }
//    emit stateChanged();
//}
