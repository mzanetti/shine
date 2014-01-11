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

#include "keystore.h"

#include <QtWidgets/QApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

#include <QDir>
#include <QDebug>
#include <QQmlComponent>
#include <QImageReader>

#include "huebridgeconnection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    HueBridgeConnection::instance();

    QQmlEngine engine;
    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));
    engine.addImportPath(QDir::currentPath() + "/../../plugin/");

    engine.rootContext()->setContextProperty("keystore", new KeyStore());

    QQmlComponent *component = new QQmlComponent(&engine);
    component->loadUrl(QStringLiteral("qml/Shine.qml"));

    if (!component->isReady()) {
        qFatal(qPrintable(component->errorString()));
        return -1;
    }

    QObject *topLevel = component->create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    if (!window) {
        qDebug() << "ApplicationWindow not found. Shine.qml must be an ApplicationWindow.";
        return -1;
    }

    qDebug() << "setting app icon" << QImageReader::supportedImageFormats();
    window->setIcon(QIcon("shine.svg"));
    window->show();
    return app.exec();
}
