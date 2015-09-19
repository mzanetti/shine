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

#ifndef HUEOBJECT_H
#define HUEOBJECT_H

#include <QObject>
#include <QTimer>

class HueObject: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool autoRefresh READ autoRefresh WRITE setAutoRefresh NOTIFY autoRefreshChanged)
public:
    HueObject(QObject *parent = 0);

    bool autoRefresh();
    void setAutoRefresh(bool autoRefresh);

public slots:
    virtual void refresh() = 0;

signals:
    void autoRefreshChanged();

private:
    QTimer m_refreshTimer;
};

#endif
