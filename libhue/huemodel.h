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

#ifndef HUEMODEL_H
#define HUEMODEL_H

#include <QAbstractListModel>
#include <QTimer>

class HueModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool autoRefresh READ autoRefresh WRITE setAutoRefresh NOTIFY autoRefreshChanged)

public:

    explicit HueModel(QObject *parent = 0);

    int count() const { return rowCount(QModelIndex()); }

    bool autoRefresh();
    void setAutoRefresh(bool autoRefresh);

public slots:
    virtual void refresh() = 0;

signals:
    void countChanged();
    void autoRefreshChanged();

private:
    QTimer m_refreshTimer;
};

#endif
