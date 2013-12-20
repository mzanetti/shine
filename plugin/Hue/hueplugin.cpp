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

// Qt
#include <QtQml/qqml.h>

// self
#include "hueplugin.h"
#include "huebridgeconnection.h"
#include "lights.h"


static QObject* hueBridgeInstance(QQmlEngine* /* engine */, QJSEngine* /* scriptEngine */)
{
    return HueBridgeConnection::instance();
}

void HuePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Hue"));

    qmlRegisterSingletonType<HueBridgeConnection>(uri, 0, 1, "HueBridge", hueBridgeInstance);
    qmlRegisterType<Lights>(uri, 0, 1, "Lights");
}
