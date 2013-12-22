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

#include "configuration.h"
#include "huebridgeconnection.h"

#include <QNetworkAccessManager>
#include <QDebug>

Configuration::Configuration(QObject *parent):
    QObject(parent)
{
    connect(HueBridgeConnection::instance(), SIGNAL(connectedBridgeChanged()), this, SLOT(refresh()));
}

void Configuration::refresh()
{
    HueBridgeConnection::instance()->get("config", this, "responseReceived");
}

QString Configuration::name()
{
    return m_name;
}

void Configuration::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
}

void Configuration::responseReceived(int id, const QVariantMap &data)
{
    Q_UNUSED(id)
    qDebug() << "got config response" << data;
}
