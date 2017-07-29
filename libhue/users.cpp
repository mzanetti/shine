/*
 * Copyright 2015 Steffen Köhler
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
 *      Steffen Köhler <mail@steffenkoehler.net>
 */

#include "users.h"
#include "user.h"

#include "huebridgeconnection.h"

#include <QDebug>

Users::Users(QObject *parent) :
    HueModel(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));

    m_busy = false;
#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Users::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Users::data(const QModelIndex &index, int role) const
{
    User *user = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return user->id();
    case RoleName:
        return user->name();
    case RoleLastUsed:
        return user->lastUsed();
    case RoleCreated:
        return user->created();
    }

    return QVariant();
}

QHash<int, QByteArray> Users::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleLastUsed, "lastUsed");
    roles.insert(RoleCreated, "created");
    return roles;
}

User *Users::get(int index) const
{
    if (index > -1 && index < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

void Users::refresh()
{
    HueBridgeConnection::instance()->get("config", this, "usersReceived");
    m_busy = true;
    emit busyChanged();
}

void Users::usersReceived(int id, const QVariant &variant)
{
    Q_UNUSED(id)
    User* newUser;
    QVariantMap users = variant.toMap().value("whitelist").toMap();

    QList<User*> removedUsers;
    foreach (User *user, m_list) {
        if (!users.contains(user->id())) {
            removedUsers.append(user);
        }
    }

    foreach (User *user, removedUsers) {
        int index = m_list.indexOf(user);
        beginRemoveRows(QModelIndex(), index, index);
        delete m_list.takeAt(index);
        endRemoveRows();
    }

    // Update existing lights's name and keep track of newly added lights
    QList <User*> newUsers;
    foreach (const QString &userId, users.keys()) {
        bool existing = false;
        foreach (User *user, m_list) {
            if (user->id() == userId) {
                existing = true;
                break;
            }
        }
        if (!existing) {
            QVariantMap userMap = users.value(userId).toMap();
            newUser = new User(userId, userMap.value("name").toString(), userMap.value("last use date").toString(), userMap.value("create date").toString());
            newUsers.append(newUser);
        }
    }

    // insert newly added lights into the model
    if (newUsers.count() > 0) {
        beginInsertRows(QModelIndex(), m_list.count(), m_list.count() + newUsers.count() - 1);
        m_list.append(newUsers);
        endInsertRows();
    }
    m_busy = false;
    emit busyChanged();
}

void Users::deleteUser(int index)
{
    HueBridgeConnection::instance()->deleteResource("config/whitelist/" + get(index)->id(), this, "deleteUserFinished");
    m_busy = true;
    emit busyChanged();
}

void Users::deleteUserFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        refresh();
    }else{
        qDebug() << "An error occured while deleting user";
    }
    m_busy = false;
    emit busyChanged();
}


bool Users::busy() const
{
    return m_busy;
}
