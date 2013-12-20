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
    qDebug() << "data called" << index.row();
    Light *light = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return light->id();
    case RoleName:
        qDebug() << "name requested" << light->name();
        return light->name();
    }

    return QVariant();
}

QHash<int, QByteArray> Lights::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
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
        Light *light = new Light(lightId.toInt());
        light->setName(lights.value(lightId).toMap().value("name").toString());
        m_list.append(light);
        qDebug() << "got light" << light->name() << light->id();
    }
    endResetModel();
}
