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

#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QAbstractListModel>

#include "huemodel.h"

class Scene: public HueModel
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int lightsCount READ lightsCount NOTIFY lightsChanged)

public:
    Scene(const QString &id, const QString &name, QObject *parent = 0);

    QString id() const;

    QString name() const;
    void setName(const QString &name);

    QList<int> lights() const;
    void setLights(const QList<int> lights);

    int lightsCount() const;
    Q_INVOKABLE int light(int index) const;

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual bool busy() const;

public slots:
    void refresh();

signals:
    void nameChanged();
    void lightsChanged();

private:
    QString m_id;
    QString m_name;
    QList<int> m_lightIds;
    bool m_busy;
};

#endif
