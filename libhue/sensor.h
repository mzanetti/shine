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

#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QAbstractListModel>
#include <QDateTime>

class Sensor: public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)
    Q_FLAGS(Types)
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(QVariantMap stateMap READ stateMap NOTIFY stateMapChanged)
    Q_PROPERTY(QString modelId READ modelId CONSTANT)
    Q_PROPERTY(QString manufacturerName READ manufacturerName CONSTANT)
    Q_PROPERTY(QString uniqueId READ uniqueId CONSTANT)

public:
    enum Type {
        TypeUnknown = 0x000,
        TypeZGPSwitch = 0x001,
        TypeZLLSwitch = 0x002,
        TypeClipSwitch = 0x004,
        TypeClipOpenClose = 0x008,
        TypeClipPresence = 0x010,
        TypeClipTemperature = 0x020,
        TypeClipHumidity = 0x040,
        TypeDaylight = 0x080,
        TypeClipGenericFlag = 0x100,
        TypeClipGenericStatus = 0x200,
        TypeAll = 0xffff
    };
    Q_DECLARE_FLAGS(Types, Type)

    Sensor(const QString &id, const QString &name, QObject *parent = 0);

    QString id() const;

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(Type type);
    QString typeString() const;

    QString modelId() const;
    void setModelId(const QString &modelId);

    QString manufacturerName() const;
    void setManufacturerName(const QString &manufacturerName);

    QString uniqueId() const;
    void setUniqueId(const QString &uniqueId);

    QVariantMap stateMap() const;
    void setStateMap(const QVariantMap &stateMap);

    static Type typeStringToType(const QString &typeString);
    static QString typeToString(Type type);

signals:
    void nameChanged();
    void typeChanged();
    void stateMapChanged();

private:
    QString m_id;
    QString m_name;
    Type m_type;
    QString m_modelId;
    QString m_manufacturerName;
    QString m_uniqueId;
    QVariantMap m_stateMap;
};

#endif
