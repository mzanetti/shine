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

#ifndef LIGHTS_H
#define LIGHTS_H

#include <QAbstractListModel>
#include <QTimer>

class Light;

class Lights : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId,
        RoleName,
        RoleModelId,
        RoleType,
        RoleSwVersion,
        RoleOn,
        RoleBrightness,
        RoleHue,
        RoleSaturation,
        RoleXY,
        RoleCt,
        RoleAlert,
        RoleEffect,
        RoleColorMode,
        RoleReachable
    };

    explicit Lights(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Light* get(int index) const;

public slots:
    void refresh();

private slots:
    void lightsReceived(int id, const QVariant &variant);
    void lightDescriptionChanged();
    void lightStateChanged();

private:
    Light* createLight(int id, const QString &name);

private:
    QList<Light*> m_list;
    QTimer m_timer;
};

#endif // LIGHTS_H
