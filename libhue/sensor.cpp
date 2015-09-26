/*
 * Copyright 2015 Michael Zanetti
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

#include "sensor.h"
#include "huebridgeconnection.h"

#include <QColor>
#include <QDebug>
#include <qabstractitemmodel.h>

Sensor::Sensor(const QString &id, const QString &name, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_name(name)
    , m_type(TypeUnknown)
{
}

QString Sensor::id() const
{
    return m_id;
}

QString Sensor::name() const
{
    return m_name;
}

void Sensor::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

Sensor::Type Sensor::type() const
{
    return m_type;
}

void Sensor::setType(Sensor::Type type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString Sensor::typeString() const
{
    return typeToString(m_type);
}

QString Sensor::modelId() const
{
    return m_modelId;
}

void Sensor::setModelId(const QString &modelId)
{
    m_modelId = modelId;
}

QString Sensor::manufacturerName() const
{
    return m_manufacturerName;
}

void Sensor::setManufacturerName(const QString &manufacturerName)
{
    m_manufacturerName = manufacturerName;
}

QString Sensor::uniqueId() const
{
    return m_uniqueId;
}

void Sensor::setUniqueId(const QString &uniqueId)
{
    m_uniqueId = uniqueId;
}

QVariantMap Sensor::stateMap() const
{
    return m_stateMap;
}

void Sensor::setStateMap(const QVariantMap &stateMap)
{
    if (m_stateMap != stateMap) {
        m_stateMap = stateMap;
        emit stateMapChanged();
    }
}

Sensor::Type Sensor::typeStringToType(const QString &typeString)
{
    if (typeString == "ZGPSwitch") {
        return TypeZGPSwitch;
    } else if (typeString == "ZLLSwitch") {
        return TypeZLLSwitch;
    } else if (typeString == "Daylight") {
        return TypeDaylight;
    } else if (typeString == "CLIPGenericStatus") {
        return TypeClipGenericStatus;
    }
    qDebug() << "Unhandled Sensor type:" << typeString;
    return TypeUnknown;
}

QString Sensor::typeToString(Sensor::Type type)
{
    switch (type) {
    case Sensor::TypeZGPSwitch:
        return "ZGPSwitch";
    case Sensor::TypeZLLSwitch:
        return "ZLLSwitch";
    case Sensor::TypeDaylight:
        return "Daylight";
    case Sensor::TypeClipGenericStatus:
        return "CLIPGenericStatus";
    }
    return QString("Unkown");
}
