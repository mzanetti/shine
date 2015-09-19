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

#include "hueobject.h"
#include "huebridgeconnection.h"

HueObject::HueObject(QObject *parent):
    QObject(parent)
{
    connect(&m_refreshTimer, SIGNAL(timeout()), this, SLOT(refresh()));
    m_refreshTimer.setInterval(10000);
}

bool HueObject::autoRefresh()
{
    return m_refreshTimer.isActive();
}

void HueObject::setAutoRefresh(bool autoRefresh)
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
