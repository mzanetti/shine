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

#ifndef LIGHTINTERFACE_H
#define LIGHTINTERFACE_H

#include "huebridgeconnection.h"

#include <QObject>
#include <QPointF>
#include <QColor>
#include <QTimer>

class LightInterface: public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorMode)

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(bool on READ on WRITE setOn NOTIFY stateChanged)
    Q_PROPERTY(quint8 bri READ bri WRITE setBri NOTIFY stateChanged)
    Q_PROPERTY(quint16 hue READ hue NOTIFY stateChanged)
    Q_PROPERTY(quint8 sat READ sat NOTIFY stateChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY stateChanged)
    Q_PROPERTY(QPointF xy READ xy NOTIFY stateChanged)
    Q_PROPERTY(quint16 ct READ ct WRITE setCt NOTIFY stateChanged)
    Q_PROPERTY(QString alert READ alert NOTIFY stateChanged)
    Q_PROPERTY(QString effect READ effect WRITE setEffect NOTIFY stateChanged)
    Q_PROPERTY(ColorMode colormode READ colorMode NOTIFY stateChanged)
    Q_PROPERTY(bool reachable READ reachable NOTIFY stateChanged)

public:
    enum ColorMode {
        ColorModeHS,
        ColorModeXY,
        ColorModeCT
    };

    LightInterface(QObject *parent)
        : QObject(parent)
    {
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(refresh()));
        m_timer.start(10000);
        connect(HueBridgeConnection::instance(), SIGNAL(stateChanged()), this, SLOT(refresh()));
    }

    virtual int id() const = 0;

    virtual QString name() const = 0;
    virtual void setName(const QString &name) = 0;

    virtual bool on() const = 0;
    virtual quint8 bri() const = 0;
    virtual quint16 hue() const = 0;
    virtual quint8 sat() const = 0;
    virtual QColor color() const = 0;
    virtual QPointF xy() const = 0;
    virtual quint16 ct() const = 0;
    virtual QString alert() const = 0;
    virtual QString effect() const = 0;
    virtual ColorMode colorMode() const = 0;
    virtual bool reachable() const = 0;

public slots:
    virtual void refresh() = 0;
    virtual void setOn(bool on) = 0;
    virtual void setBri(quint8 bri) = 0;
    virtual void setHue(quint16 hue) = 0;
    virtual void setSat(quint8 sat) = 0;
    virtual void setColor(const QColor &color) = 0;
    virtual void setXy(const QPointF &xy) = 0;
    virtual void setCt(quint16 ct) = 0;
    virtual void setAlert(const QString &alert) = 0;
    virtual void setEffect(const QString &effect) = 0;

signals:
    void nameChanged();
    void stateChanged();

private:
    QTimer m_timer;
};

#endif
