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

#include "huemodel.h"
#include "iconMap.h"

#include <QTimer>

class Light;

class Lights : public HueModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId = Qt::UserRole,
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
        RoleReachable,
        RoleIcon,
        RoleIconOutline
    };

    explicit Lights(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Light* get(int index) const;
    Q_INVOKABLE Light* findLight(int lightId) const;

    Q_INVOKABLE void searchForNewLights();

    bool busy() const;

public slots:
    void refresh();

private slots:
    void lightsReceived(int id, const QVariant &variant);
    void lightDescriptionChanged();
    void lightStateChanged();
    void searchStarted(int id, const QVariant &response);

signals:
    void countChanged();

private:
    Light* createLight(int id, const QString &name);
    void parseStateMap(Light *light, const QVariantMap &stateMap);

private:
    QList<Light*> m_list;
    bool m_busy;
    IconMap *iconMap;
};

#endif // LIGHTS_H
