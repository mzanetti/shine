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

#include "scene.h"
#include "huebridgeconnection.h"

#include <QColor>
#include <QDebug>
#include <qabstractitemmodel.h>

Scene::Scene(const QString &id, const QString &name, QObject *parent)
    : HueModel(parent)
    , m_id(id)
    , m_name(name)
{
//    refresh();
}

QString Scene::id() const
{
    return m_id;
}

QString Scene::name() const
{
    return m_name;
}

void Scene::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

QList<int> Scene::lights() const
{
    return m_lightIds;
}

void Scene::setLights(const QList<int> lights)
{
    if (lights != m_lightIds) {
        // TODO: do an actual merge instead of clearing and readding
        beginRemoveRows(QModelIndex(), 0, m_lightIds.size()-1);
        m_lightIds.clear();
        endRemoveRows();

        beginInsertRows(QModelIndex(), 0, lights.size()-1);
        m_lightIds = lights;
        endInsertRows();
        emit lightsChanged();
    }
}

int Scene::light(int index) const
{
    if (index < 0 || index >= m_lightIds.count()) {
        return -1;
    }
    return m_lightIds.at(index);
}

bool Scene::containsLight(int index) const
{
    return m_lightIds.contains(index);
}

int Scene::lightsCount() const
{
    return m_lightIds.count();
}

void Scene::refresh()
{
}


int Scene::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_lightIds.count();
}

QVariant Scene::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole){
        return m_lightIds.at(index.row());
    }
    return QVariant();
}

bool Scene::busy() const
{
    return m_busy;
}
