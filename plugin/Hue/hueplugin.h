/*
 * Copyright (C) 2013 Michael Zanetti
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Michael Zanetti <michael_zanetti@gmx.net>
 */

#ifndef HUEPLUGIN_H
#define HUEPLUGIN_H

#include <qglobal.h>

#if QT_VERSION >= 0x050000
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlExtensionPlugin>

class HuePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

#else

#include <QDeclarativeExtensionPlugin>

class HuePlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

#endif


public:
    void registerTypes(const char *uri);

#if QT_VERSION < 0x050000
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);
#endif
};

#endif
