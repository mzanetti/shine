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

#ifndef SCENES_H
#define SCENES_H

#include "huemodel.h"

class Scene;

class Scenes: public HueModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId,
        RoleName
    };

    explicit Scenes(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Scene* get(int index) const;
    Q_INVOKABLE Scene *findScene(const QString &id) const;

    Q_INVOKABLE void recallScene(const QString &id);

    bool busy() const;

public slots:
    Q_INVOKABLE void createScene(const QString &name, const QList<int> &lights);
    Q_INVOKABLE void updateScene(const QString &id, const QString &name, const QList<int> &lights);

    void refresh();

private slots:
    void createSceneFinished(int id, const QVariant &variant);
    void recallSceneFinished(int id, const QVariant &variant);
//    void deleteGroupFinished(int id, const QVariant &variant);
    void scenesReceived(int id, const QVariant &variant);
    void sceneNameChanged();
//    void groupLightsChanged();

private:
    Scene* createSceneInternal(const QString &id, const QString &name, const QList<int> lights);

    QList<Scene*> m_list;
    bool m_busy;
};

#endif // SCENES_H
