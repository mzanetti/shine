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

#include "keystore.h"

#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QObject>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    QStringList imports = view.engine()->importPathList();
    // For easy running in build dir
    imports.append(QDir::currentPath() + "/../../plugin/");
    // For finding plugins in the click package
    imports.append(QDir::currentPath());

    view.engine()->setImportPathList(imports);

    view.engine()->rootContext()->setContextProperty("keystore", new KeyStore());

    QObject::connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));

    view.setSource(QStringLiteral("qml/Shine.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
