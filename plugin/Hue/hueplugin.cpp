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

#include "hueplugin.h"

// FIXME: Dafuq is going on on harmattan? doesnt find those with include_dir() in CMakeLists.txt
#include "../../libhue/huebridgeconnection.h"
#include "../../libhue/lights.h"
#include "../../libhue/light.h"
#include "../../libhue/groups.h"
#include "../../libhue/group.h"
#include "../../libhue/scenes.h"
#include "../../libhue/scene.h"
#include "../../libhue/schedules.h"
#include "../../libhue/schedule.h"
#include "../../libhue/lightsfiltermodel.h"
#include "../../libhue/scenesfiltermodel.h"
#include "../../libhue/configuration.h"
#include "../../libhue/sensor.h"
#include "../../libhue/sensors.h"
#include "../../libhue/sensorsfiltermodel.h"
#include "../../libhue/rule.h"
#include "../../libhue/rules.h"
#include "../../libhue/rulesfiltermodel.h"

#if QT_VERSION >= 0x050000
#include <QtQml/qqml.h>
#else
#include <QtDeclarative>
#include <QDeclarativeContext>
#endif


#if QT_VERSION >= 0x050000
static QObject* hueBridgeInstance(QQmlEngine* /* engine */, QJSEngine* /* scriptEngine */)
{
    return HueBridgeConnection::instance();
}
#endif

void HuePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Hue"));

#if QT_VERSION >= 0x050000
    qmlRegisterSingletonType<HueBridgeConnection>(uri, 0, 1, "HueBridge", hueBridgeInstance);
#endif
    qmlRegisterType<Lights>(uri, 0, 1, "Lights");
    qmlRegisterUncreatableType<Light>(uri, 0, 1, "Light", "Cannot create lights. Get them from the Lights model.");
    qmlRegisterUncreatableType<LightInterface>(uri, 0, 1, "LightInterface", "Abstract interface.");
    qmlRegisterType<LightsFilterModel>(uri, 0, 1, "LightsFilterModel");
    qmlRegisterType<Groups>(uri, 0, 1, "Groups");
    //FIXME: eventually creatable
    qmlRegisterUncreatableType<Group>(uri, 0, 1, "Group", "Cannot create groups. Get them from the Groups model.");
    qmlRegisterType<Scenes>(uri, 0, 1, "Scenes");
    qmlRegisterUncreatableType<Scene>(uri, 0, 1, "Scene", "Cannot create Scene objects. Get them from the Scenes model.");
    qmlRegisterType<ScenesFilterModel>(uri, 0, 1, "ScenesFilterModel");
    qmlRegisterType<Schedules>(uri, 0, 1, "Schedules");
    qmlRegisterUncreatableType<Schedule>(uri, 0, 1, "Schedule", "Cannot create Schedule objects. Get them from the Schedules model.");
    qmlRegisterType<Configuration>(uri, 0, 1, "Configuration");
    qmlRegisterType<Sensors>(uri, 0, 1, "Sensors");
    qmlRegisterType<SensorsFilterModel>(uri, 0, 1, "SensorsFilterModel");
    qmlRegisterUncreatableType<Sensor>(uri, 0, 1, "Sensor", "Cannot create Sensor objects. Get them from the Sensors model.");
    qmlRegisterType<Rules>(uri, 0, 1, "Rules");
    qmlRegisterType<RulesFilterModel>(uri, 0, 1, "RulesFilterModel");
    qmlRegisterUncreatableType<Rule>(uri, 0, 1, "Rule", "Cannot create Rule objects. Get them from the Rules model.");
}


#if QT_VERSION < 0x050000
void HuePlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    engine->rootContext()->setContextProperty("hueBridge", HueBridgeConnection::instance());
}

Q_EXPORT_PLUGIN2(Hue, HuePlugin)
#endif
