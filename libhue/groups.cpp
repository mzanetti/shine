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
    : QAbstractListModel(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
    refresh();

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
    return m_list.at(index);
}

void Groups::refresh()
{
    HueBridgeConnection::instance()->get("groups", this, "groupsReceived");
}

void Groups::groupsReceived(int id, const QVariant &variant)
{
    Q_UNUSED(id)
    qDebug() << "got groups" << variant;
    QVariantMap groups = variant.toMap();

    beginResetModel();
    foreach (const QString &groupId, groups.keys()) {
        Group *group = createGroup(groupId.toInt(), groups.value(groupId).toMap().value("name").toString());
        m_list.append(group);
        qDebug() << "got group" << group->name() << group->id();
    }
    endResetModel();
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

Group *Groups::createGroup(int id, const QString &name)
{
    Group *group = new Group(id, name);

    connect(group, SIGNAL(nameChanged()), this, SLOT(groupDescriptionChanged()));
    connect(group, SIGNAL(stateChanged()), this, SLOT(groupStateChanged()));

    return group;
}
