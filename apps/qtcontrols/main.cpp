/*
 * Copyright 2013 Michael Zanetti
 *                Christian Muehlhaeuser
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
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>

#include <QDir>
#include <QDebug>
#include <QQmlComponent>

#include "huebridgeconnection.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    HueBridgeConnection::instance();

    QQmlEngine engine;
    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    engine.addImportPath(QDir::currentPath() + "/../../plugin/");

    QQmlComponent *component = new QQmlComponent(&engine);
//    if (!component->isReady()) {
//        qFatal(qPrintable("Failed to load QML. exiting"));
//        return -1;
//    }

    component->loadUrl(QStringLiteral("qml/Shine.qml"));
    if (!component->isReady()) {
        qFatal(qPrintable(component->errorString()));
        return -1;
    }

    QObject *topLevel = component->create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);
    window->setIcon(QIcon(":/data/images/shine.svg"));

    if (!window) {
        qDebug() << "ApplicationWindow not found. Shine.qml must be an ApplicationWindow.";
        return -1;
    }

    window->show();

    return app.exec();
}
