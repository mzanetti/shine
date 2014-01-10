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

#include <QtGui/QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>

#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;

    // For easy running
    QStringList imports = view.engine()->importPathList();
    imports.append(QDir::currentPath() + "/../../plugin/");
    view.engine()->setImportPathList(imports);

    view.setSource(QStringLiteral("qml/Shine.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();


    return app.exec();
}
