#include "lights.h"
#include "light.h"

#include "huebridgeconnection.h"

#include <QDebug>

Lights::Lights(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(HueBridgeConnection::instance(), &HueBridgeConnection::usernameChanged, this, &Lights::refresh);
    refresh();
}

int Lights::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Lights::data(const QModelIndex &index, int role) const
{
    qDebug() << "data called" << index.row() << role << RoleOn;
    Light *light = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return light->id();
    case RoleName:
        qDebug() << "name requested" << light->name();
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

void Lights::refresh()
{
    HueBridgeConnection::instance()->get("lights", this, "lightsReceived");
}

void Lights::lightsReceived(int id, const QVariant &variant)
{
    qDebug() << "got lights" << variant;
    QVariantMap lights = variant.toMap();

    beginResetModel();
    foreach (const QString &lightId, lights.keys()) {
        Light *light = createLight(lightId.toInt());
        QVariantMap lightMap = lights.value(lightId).toMap();
        light->setName(lightMap.value("name").toString());
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
    QVector<int> roles = QVector<int>()
            << RoleId
            << RoleName
            << RoleModelId
            << RoleType
            << RoleSwVersion;

    emit dataChanged(modelIndex, modelIndex, roles);
}

void Lights::lightStateChanged()
{
    Light *light = static_cast<Light*>(sender());
    int idx = m_list.indexOf(light);
    QModelIndex modelIndex = index(idx);
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
}

Light *Lights::createLight(int id)
{
    Light *light = new Light(id);

    connect(light, &Light::nameChanged, this, &Lights::lightDescriptionChanged);
    connect(light, &Light::modelIdChanged, this, &Lights::lightDescriptionChanged);
    connect(light, &Light::typeChanged, this, &Lights::lightDescriptionChanged);
    connect(light, &Light::swversionChanged, this, &Lights::lightDescriptionChanged);

    connect(light, &Light::stateChanged, this, &Lights::lightStateChanged);

    return light;
}
