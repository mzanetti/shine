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

#include "lights.h"
#include "light.h"

#include "huebridgeconnection.h"

#include <QDebug>

Lights::Lights(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
    refresh();

#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Lights::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Lights::data(const QModelIndex &index, int role) const
{
    Light *light = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return light->id();
    case RoleName:
        return light->name();
    case RoleModelId:
        return light->modelId();
    case RoleType:
        return light->type();
    case RoleSwVersion:
        return light->swversion();
    case RoleOn:
        return light->on();
    case RoleBrightness:
        return light->bri();
    case RoleHue:
        return light->hue();
    case RoleSaturation:
        return light->sat();
    case RoleXY:
        return light->xy();
    case RoleAlert:
        return light->alert();
    case RoleEffect:
        return light->effect();
    case RoleColorMode:
        return light->colormode();
    case RoleReachable:
        return light->reachable();
    }

    return QVariant();
}

QHash<int, QByteArray> Lights::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleModelId, "modelid");
    roles.insert(RoleType, "type");
    roles.insert(RoleSwVersion, "swversion");
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

Light *Lights::get(int index) const
{
    return m_list.at(index);
}

void Lights::refresh()
{
    HueBridgeConnection::instance()->get("lights", this, "lightsReceived");
}

void Lights::lightsReceived(int id, const QVariant &variant)
{
    Q_UNUSED(id)
    qDebug() << "got lights" << variant;
    QVariantMap lights = variant.toMap();

    beginResetModel();
    foreach (const QString &lightId, lights.keys()) {
        Light *light = createLight(lightId.toInt(), lights.value(lightId).toMap().value("name").toString());
        m_list.append(light);
        qDebug() << "got light" << light->name() << light->id();
    }
    endResetModel();
}

void Lights::lightDescriptionChanged()
{
    Light *light = static_cast<Light*>(sender());
    int idx = m_list.indexOf(light);
    QModelIndex modelIndex = index(idx);

#if QT_VERSION >= 0x050000
    QVector<int> roles = QVector<int>()
            << RoleId
            << RoleName
            << RoleModelId
            << RoleType
            << RoleSwVersion;

    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Lights::lightStateChanged()
{
    Light *light = static_cast<Light*>(sender());
    int idx = m_list.indexOf(light);
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

Light *Lights::createLight(int id, const QString &name)
{
    Light *light = new Light(id, name);

    connect(light, SIGNAL(nameChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(modelIdChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(typeChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(swversionChanged()), this, SLOT(lightDescriptionChanged()));

    connect(light, SIGNAL(stateChanged()), this, SLOT(lightStateChanged()));

    return light;
}
