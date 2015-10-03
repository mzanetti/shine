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
    HueModel(parent),
    m_busy(false)
{
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
    case RoleCt:
        return light->ct();
    case RoleAlert:
        return light->alert();
    case RoleEffect:
        return light->effect();
    case RoleColorMode:
        return light->colorMode();
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
    roles.insert(RoleCt, "ct");
    roles.insert(RoleAlert, "alert");
    roles.insert(RoleEffect, "effect");
    roles.insert(RoleColorMode, "colormode");
    roles.insert(RoleReachable, "reachable");
    return roles;
}

Light *Lights::get(int index) const
{
    if (index > -1 && index < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Light *Lights::findLight(int lightId) const
{
    foreach (Light *light, m_list) {
        if (light->id() == lightId) {
            return light;
        }
    }
    return 0;
}

void Lights::searchForNewLights()
{
    HueBridgeConnection::instance()->post("lights", QVariantMap(), this, "searchStarted");
}

bool Lights::busy() const
{
    return m_busy;
}

void Lights::refresh()
{
    HueBridgeConnection::instance()->get("lights", this, "lightsReceived");
    m_busy = true;
    emit busyChanged();
}

void Lights::lightsReceived(int id, const QVariant &variant)
{
    qDebug() << "lightsResponse" << variant;
    Q_UNUSED(id)
    QVariantMap lights = variant.toMap();

    // Find removed lights
    QList<Light*> removedLights;
    foreach (Light *light, m_list) {
        if (!lights.keys().contains(QString::number(light->id()))) {
            removedLights.append(light);
        }
    }

    // Remove removed lights from the model
    foreach (Light *light, removedLights) {
        int index = m_list.indexOf(light);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    // Update existing lights's name and keep track of newly added lights
    QList <Light*> newLights;
    foreach (const QString &lightId, lights.keys()) {
        Light *light = findLight(lightId.toInt());
        if (light) {
            light->m_name = lights.value(lightId).toMap().value("name").toString();
            light->m_modelId = lights.value(lightId).toMap().value("modelid").toString();
        } else {
            light = createLight(lightId.toInt(), lights.value(lightId).toMap().value("name").toString());
            light->m_modelId = lights.value(lightId).toMap().value("modelid").toString();
            newLights.append(light);
        }
        qDebug() << "have modelid" << light->m_modelId;
        QVariantMap stateMap = lights.value(lightId).toMap().value("state").toMap();
        parseStateMap(light, stateMap);
    }

    // insert newly added lights into the model
    if (newLights.count() > 0) {
        beginInsertRows(QModelIndex(), m_list.count(), m_list.count() + newLights.count() - 1);
        m_list.append(newLights);
        endInsertRows();
    }

    m_busy = false;
    emit busyChanged();
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
            << RoleCt
            << RoleAlert
            << RoleEffect
            << RoleColorMode
            << RoleReachable;

    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Lights::searchStarted(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "search started" << response;
}

Light *Lights::createLight(int id, const QString &name)
{
    Light *light = new Light(id, name, this);

    connect(light, SIGNAL(nameChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(modelIdChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(typeChanged()), this, SLOT(lightDescriptionChanged()));
    connect(light, SIGNAL(swversionChanged()), this, SLOT(lightDescriptionChanged()));

    connect(light, SIGNAL(stateChanged()), this, SLOT(lightStateChanged()));

    return light;
}

void Lights::parseStateMap(Light *light, const QVariantMap &stateMap)
{
    light->m_on = stateMap.value("on").toBool();
    light->m_bri = stateMap.value("bri").toInt();
    light->m_hue = stateMap.value("hue").toInt();
    light->m_sat = stateMap.value("sat").toInt();
    light->m_xy = stateMap.value("xy").toPointF();
    light->m_ct = stateMap.value("ct").toInt();
    light->m_alert = stateMap.value("alert").toString();
    light->m_effect = stateMap.value("effect").toString();
    QString colorModeString = stateMap.value("colormode").toString();
    if (colorModeString == "hs") {
        light->m_colormode = Light::ColorModeHS;
    } else if (colorModeString == "xy") {
        light->m_colormode = Light::ColorModeXY;
    } else if (colorModeString == "ct") {
        light->m_colormode = Light::ColorModeCT;
    }
    light->m_reachable = stateMap.value("reachable").toBool();
    emit light->stateChanged();
}
