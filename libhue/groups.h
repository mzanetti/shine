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

#ifndef GROUPS_H
#define GROUPS_H

#include <QAbstractListModel>

class Group;

class Groups : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        RoleId,
        RoleName,
        RoleOn,
        RoleBrightness,
        RoleHue,
        RoleSaturation,
        RoleXY,
        RoleAlert,
        RoleEffect,
        RoleColorMode,
        RoleReachable,
        RoleLightIds
    };

    explicit Groups(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int count() const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Group* get(int index) const;

public slots:
    Q_INVOKABLE void createGroup(const QString &name, const QList<int> &lights);
    Q_INVOKABLE void deleteGroup(int id);

    void refresh();

signals:
    void countChanged();

private slots:
    void createGroupFinished(int id, const QVariant &variant);
    void deleteGroupFinished(int id, const QVariant &variant);
    void groupsReceived(int id, const QVariant &variant);
    void groupDescriptionChanged();
    void groupStateChanged();
    void groupLightsChanged();

private:
    Group* createGroupInternal(int id, const QString &name);

    QList<Group*> m_list;
};

#endif // GROUPS_H
