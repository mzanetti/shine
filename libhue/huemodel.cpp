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

#include "huemodel.h"
#include "huebridgeconnection.h"

HueModel::HueModel(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(&m_refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    m_refreshTimer.setInterval(10000);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(modelReset()), this, SIGNAL(countChanged()));
}

bool HueModel::autoRefresh() const
{
    return m_refreshTimer.isActive();
}

void HueModel::setAutoRefresh(bool autoRefresh)
{
    if (m_refreshTimer.isActive() && !autoRefresh) {
        m_refreshTimer.stop();
        disconnect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
        emit autoRefreshChanged();
    } else if (!m_refreshTimer.isActive() && autoRefresh) {
        m_refreshTimer.start();
        connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
        refresh();
        emit autoRefreshChanged();
    }
}
