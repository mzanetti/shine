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

#include "scenes.h"
#include "scene.h"

#include "huebridgeconnection.h"

#include <QDebug>
#include <QUuid>

Scenes::Scenes(QObject *parent):
    HueModel(parent),
    m_busy(false)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Scenes::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Scenes::data(const QModelIndex &index, int role) const
{
    Scene *scene= m_list.at(index.row());
    switch (role) {
    case RoleId:
        return scene->id();
    case RoleName:
        return scene->name();
    }

    return QVariant();
}

QHash<int, QByteArray> Scenes::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    return roles;
}

Scene *Scenes::get(int index) const
{
    if (index > -1 && index  < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Scene *Scenes::findScene(const QString &id) const
{
    foreach (Scene *scene, m_list) {
        if (scene->id() == id) {
            return scene;
        }
    }
    return 0;
}

void Scenes::recallScene(const QString &id)
{
    QVariantMap params;
    params.insert("scene", id);
    HueBridgeConnection::instance()->put("groups/0/action", params, this, "recallSceneFinished");
}

bool Scenes::busy() const
{
    return m_busy;
}

void Scenes::refresh()
{
    HueBridgeConnection::instance()->get("scenes", this, "scenesReceived");
    m_busy = true;
    emit busyChanged();
}

void Scenes::scenesReceived(int id, const QVariant &variant)
{
//    qDebug() << "**** scenes received" << variant;
    Q_UNUSED(id)
    QVariantMap scenes = variant.toMap();
    QList<Scene*> removedScenes;
    foreach (Scene *scene, m_list) {
        if (!scenes.contains(scene->id())) {
//            qDebug() << "removing scene" << scene->id();
            removedScenes.append(scene);
        } else {
//            qDebug() << "updating scene" << scene->id();
            QVariantMap sceneMap = scenes.value(scene->id()).toMap();
            scene->setName(sceneMap.value("name").toString());
            QList<int> lights;
            foreach (const QVariant &light, sceneMap.value("lights").toList()) {
                lights << light.toInt();
            }
            scene->setLights(lights);
        }
    }

    foreach (Scene *scene, removedScenes) {
        int index = m_list.indexOf(scene);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &sceneId, scenes.keys()) {
        if (findScene(sceneId) == 0) {
            QVariantMap sceneMap = scenes.value(sceneId).toMap();
            QList<int> lights;
            foreach (const QVariant &light, sceneMap.value("lights").toList()) {
                lights << light.toInt();
            }
            createSceneInternal(sceneId, sceneMap.value("name").toString(), lights);
//            qDebug() << "creating scene with lights" << lights << scene->lightsCount();
        }
    }

    m_busy = false;
    emit busyChanged();
}

void Scenes::sceneNameChanged()
{
    Scene *scene = static_cast<Scene*>(sender());
    int idx = m_list.indexOf(scene);
    QModelIndex modelIndex = index(idx);

#if QT_VERSION >= 0x050000
    QVector<int> roles = QVector<int>()
            << RoleName;

    emit dataChanged(modelIndex, modelIndex, roles);
#else
    emit dataChanged(modelIndex, modelIndex);
#endif
}

void Scenes::createScene(const QString &name, const QList<int> &lights)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i < 10; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    updateScene("shine" + randomString, name, lights);
}

void Scenes::updateScene(const QString &id, const QString &name, const QList<int> &lights)
{
    qDebug() << "create scene" << name << lights;
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

Scene *Scenes::createSceneInternal(const QString &id, const QString &name, const QList<int> lights)
{
    Scene *scene = new Scene(id, name, this);
    scene->setLights(lights);

    connect(scene, SIGNAL(nameChanged()), this, SLOT(sceneNameChanged()));

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(scene);
    endInsertRows();
    return scene;
}

void Scenes::createSceneFinished(int id, const QVariant &response)
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

void Scenes::recallSceneFinished(int id, const QVariant &variant)
{
    qDebug() << "scene recalled" << variant;
}
