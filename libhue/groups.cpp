/*
 * Copyright 2013 Christian Muehlhaeuser
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
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "groups.h"
#include "group.h"

#include "huebridgeconnection.h"

#include <QDebug>

Groups::Groups(QObject *parent)
    : HueModel(parent),
      m_busy(false)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Groups::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Groups::data(const QModelIndex &index, int role) const
{
    Group *group = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return group->id();
    case RoleName:
        return group->name();
    case RoleOn:
        return group->on();
    case RoleBrightness:
        return group->bri();
    case RoleHue:
        return group->hue();
    case RoleSaturation:
        return group->sat();
    case RoleXY:
        return group->xy();
    case RoleAlert:
        return group->alert();
    case RoleEffect:
        return group->effect();
    case RoleColorMode:
        return group->colorMode();
    case RoleReachable:
        return group->reachable();
    }

    return QVariant();
}

QHash<int, QByteArray> Groups::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleOn, "on");
    roles.insert(RoleBrightness, "bri");
    roles.insert(RoleHue, "hue");
    roles.insert(RoleSaturation, "sat");
    roles.insert(RoleXY, "xy");
    roles.insert(RoleAlert, "alert");
    roles.insert(RoleEffect, "effect");
    roles.insert(RoleColorMode, "colormode");
    roles.insert(RoleReachable, "reachable");
    return roles;
}

Group *Groups::get(int index) const
{
    if (index > -1 && index  < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Group *Groups::findGroup(int id) const
{
    foreach (Group *group, m_list) {
        if (group->id() == id) {
            return group;
        }
    }
    return 0;
}

bool Groups::busy() const
{
    return m_busy;
}

void Groups::refresh()
{
    HueBridgeConnection::instance()->get("lights", this, "lightsReceived");
    m_busy = true;
    emit busyChanged();
}

void Groups::groupsReceived(int id, const QVariant &variant)
{
//    qDebug() << "groups receied" << variant;

    Q_UNUSED(id)

    // Group 0 is not included in the get all groups call !!??
    Group* group0 = findGroup(0);
    if (!group0) {
         group0 = createGroupInternal(0, "All");
    }
    group0->refresh();


    QVariantMap groups = variant.toMap();
    QList<Group*> removedGroups;
    foreach (Group *group, m_list) {
        if (group->id() != 0 && !groups.contains(QString::number(group->id()))) {
            removedGroups.append(group);
        }
    }

    foreach (Group *group, removedGroups) {
        int index = m_list.indexOf(group);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &groupId, groups.keys()) {
        Group* group = findGroup(groupId.toInt());
        if (!group) {
            group = createGroupInternal(groupId.toInt(), groups.value(groupId).toMap().value("name").toString());
        }
        parseStateMap(group, groups.value(groupId).toMap().value("action").toMap());
        group->m_on = false;
        QList<int> lightIds;
        foreach (const QVariant &lightId, groups.value(groupId).toMap().value("lights").toList()) {
            lightIds.append(lightId.toInt());
            if (m_lights.value(lightId.toInt())) {
                group->m_on = true;
            }
        }
        if (group->m_lightIds != lightIds) {
            group->m_lightIds = lightIds;
            emit group->lightsChanged();
        }
        emit group->stateChanged();
    }
    m_busy = false;
    emit busyChanged();
}

void Groups::groupDescriptionChanged()
{
    Group *group = static_cast<Group*>(sender());
    int idx = m_list.indexOf(group);
    QModelIndex modelIndex = index(idx);

#if QT_VERSION >= 0x050000
    QVector<int> roles = QVector<int>()
            << RoleId
            << RoleName;

    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Groups::groupStateChanged()
{
    Group *group = static_cast<Group*>(sender());
    int idx = m_list.indexOf(group);
    QModelIndex modelIndex = index(idx);

#if QT_VERSION >= 0x050000
    QVector<int> roles = QVector<int>()
            << RoleOn
            << RoleBrightness
            << RoleHue
            << RoleSaturation
            << RoleXY
            << RoleAlert
            << RoleEffect
            << RoleColorMode
            << RoleReachable;

    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Groups::groupLightsChanged()
{
    Group *group = static_cast<Group*>(sender());
    int idx = m_list.indexOf(group);
    QModelIndex modelIndex = index(idx);

#if QT_VERSION >= 0x050000
    QVector<int> roles = QVector<int>() << RoleLightIds;
    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Groups::createGroup(const QString &name, const QList<int> &lights)
{
    qDebug() << "create group" << name << lights;
    QVariantMap params;
    QVariantList lightsList;
    foreach (int lightId, lights) {
        qDebug() << "got light" << lightId;
        lightsList.append(QString::number(lightId));
    }
    qDebug() << "lightslist" << lightsList;
    params.insert("name", name);
    params.insert("lights", lightsList);
    HueBridgeConnection::instance()->post("groups", params, this, "createGroupFinished");
}

Group *Groups::createGroupInternal(int id, const QString &name)
{
    Group *group = new Group(id, name, this);

    connect(group, SIGNAL(nameChanged()), this, SLOT(groupDescriptionChanged()));
    connect(group, SIGNAL(stateChanged()), this, SLOT(groupStateChanged()));
    connect(group, SIGNAL(lightsChanged()), this, SLOT(groupLightsChanged()));

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(group);
    endInsertRows();
    return group;
}

void Groups::createGroupFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "got createGroup result" << response;

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        QVariantMap successMap = result.value("success").toMap();
        if (successMap.contains("id")) {
            QString groupId = successMap.value("id").toString();
            groupId = groupId.mid(groupId.lastIndexOf("/") + 1);

            //TODO: could be added without refrshing, but we don't know the name at this point.
            //TODO: might be best to ctor groups/lights with id only and make them fetch their own info.
            refresh();
        }
    }
}

void Groups::deleteGroup(int id)
{
    HueBridgeConnection::instance()->deleteResource("groups/" + QString::number(id), this, "deleteGroupFinished");
}

void Groups::deleteGroupFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "got deleteGroup result" << response;

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        QString success = result.value("success").toString();
        if (success.contains("/groups/")) {
            QString groupId = success.mid(success.indexOf("/groups/") + 8);
            groupId = groupId.left(groupId.indexOf(" deleted"));

            //TODO: could be deleted without refrshing
            refresh();
        }
    }
}

void Groups::lightsReceived(int id, const QVariant &variant)
{
    Q_UNUSED(id)
//    qDebug() << "lights received" << variant;

    m_lights.clear();
    foreach (const QString &lightId, variant.toMap().keys()) {
        m_lights.insert(lightId.toInt(), variant.toMap().value(lightId).toMap().value("state").toMap().value("on").toBool());
    }

    HueBridgeConnection::instance()->get("groups", this, "groupsReceived");
}

void Groups::parseStateMap(Group *group, const QVariantMap &stateMap)
{
    group->m_on = stateMap.value("on").toBool();
    group->m_bri = stateMap.value("bri").toInt();
    group->m_hue = stateMap.value("hue").toInt();
    group->m_sat = stateMap.value("sat").toInt();
    group->m_xy = stateMap.value("xy").toPointF();
    group->m_ct = stateMap.value("ct").toInt();
    group->m_alert = stateMap.value("alert").toString();
    group->m_effect = stateMap.value("effect").toString();
    QString colorModeString = stateMap.value("colormode").toString();
    if (colorModeString == "hs") {
        group->m_colormode = Group::ColorModeHS;
    } else if (colorModeString == "xy") {
        group->m_colormode = Group::ColorModeXY;
    } else if (colorModeString == "ct") {
        group->m_colormode = Group::ColorModeCT;
    }
    group->m_reachable = true;//stateMap.value("reachable").toBool();

}
