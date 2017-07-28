/*****************************************************************************
 * Copyright: 2017      Michael Zanetti <michael_zanetti@gmx.net>            *
 *                                                                           *
 * This file is part of Shine                                                *
 *                                                                           *
 * Shine is free software: you can redistribute it and/or modify             *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, version 3 of the License.                   *
 *                                                                           *
 * Shine is distributed in the hope that it will be useful,                  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

#include <QGuiApplication>

#include <QDebug>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

#include "keystore.h"

int main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication application(argc, argv);

    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;

    QStringList imports = engine.importPathList();
    // For easy running in build dir
    imports.append(QDir::currentPath() + "/../../plugin/");
    // For finding plugins in the package
    imports.append(QDir::currentPath());
    engine.setImportPathList(imports);

    engine.rootContext()->setContextProperty("keystore", new KeyStore());
    engine.load(QUrl("qrc:///qml/Shine.qml"));

    return application.exec();
}

