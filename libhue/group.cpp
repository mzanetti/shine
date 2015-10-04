/*
 * Copyright 2013 Christian Muehlhaeuser
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
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "group.h"
#include "huebridgeconnection.h"

#include <QColor>
#include <QDebug>
#include <qabstractitemmodel.h>
#include <QGenericMatrix>

Group::Group(int id, const QString &name, QObject *parent)
    : LightInterface(parent)
    , m_id(id)
    , m_name(name)
    , m_bri(0),
    m_busyStateChangeId(-1),
    m_hueDirty(false),
    m_satDirty(false),
    m_briDirty(false),
    m_ctDirty(false),
    m_xyDirty(false)
{
    m_timeout.setInterval(250);
    connect(&m_timeout, &QTimer::timeout, this, &Group::timeout);
}

int Group::id() const
{
    return m_id;
}

QString Group::name() const
{
    return m_name;
}

void Group::setName(const QString &name)
{
    if (m_name != name) {
        QVariantMap params;
        params.insert("name", name);
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id), params, this, "setDescriptionFinished");
    }
}

bool Group::on() const
{
    return m_on;
}

void Group::setOn(bool on)
{
    QVariantMap params;
    params.insert("on", on);
    HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
}

quint8 Group::bri() const
{
    return m_bri;
}

void Group::setBri(quint8 bri)
{
    if (bri != m_bri) {
        QVariantMap params;
        params.insert("on", true);
        params.insert("bri", bri);
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
    }
}

quint16 Group::hue() const
{
    return m_hue;
}

void Group::setHue(quint16 hue)
{
    if (hue != m_hue) {
        QVariantMap params;
        params.insert("on", true);
        params.insert("hue", hue);
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
    }
}

quint8 Group::sat() const
{
    return m_sat;
}

void Group::setSat(quint8 sat)
{
    if (sat != m_sat) {
        QVariantMap params;
        params.insert("on", true);
        params.insert("sat", sat);
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
    }
}

QColor Group::color() const
{
    return QColor::fromHsv(hue() * 360 / 65535, sat(), 255);
}

void Group::setColor(const QColor &color)
{
    // Transform from RGB to Hue/Sat
    quint16 hue = color.hue() * 65535 / 360;
    quint8 sat = color.saturation();

    // Transform from RGB to XYZ
    QGenericMatrix<3, 3, qreal> rgb2xyzMatrix;
    rgb2xyzMatrix(0, 0) = 0.412453;    rgb2xyzMatrix(0, 1) = 0.357580;    rgb2xyzMatrix(0, 2) = 0.180423;
    rgb2xyzMatrix(1, 0) = 0.212671;    rgb2xyzMatrix(1, 1) = 0.715160;    rgb2xyzMatrix(1, 2) = 0.072169;
    rgb2xyzMatrix(2, 0) = 0.019334;    rgb2xyzMatrix(2, 1) = 0.119193;    rgb2xyzMatrix(2, 2) = 0.950227;

    QGenericMatrix<1, 3, qreal> rgbMatrix;
    rgbMatrix(0, 0) = 1.0 * color.red() / 255;
    rgbMatrix(1, 0) = 1.0 * color.green() / 255;
    rgbMatrix(2, 0) = 1.0 * color.blue() / 255;

    QGenericMatrix<1, 3, qreal> xyzMatrix = rgb2xyzMatrix * rgbMatrix;

    // transform from XYZ to CIELUV u' and v'
    qreal u = 4*xyzMatrix(0, 0) / (xyzMatrix(0, 0) + 15*xyzMatrix(1, 0) + 3*xyzMatrix(2, 0));
    qreal v = 9*xyzMatrix(1, 0) / (xyzMatrix(0, 0) + 15*xyzMatrix(1, 0) + 3*xyzMatrix(2, 0));

    // Transform from CIELUV to (x,y)
    qreal x = 27*u / (18*u - 48*v + 36);
    qreal y = 12*v / (18*u - 48*v + 36);

    qDebug() << "setting color" << color << x << y;
    if (m_busyStateChangeId == -1) {
        QVariantMap params;

        params.insert("hue", hue);
        params.insert("sat", sat);
        // FIXME: There is a bug in the API that it doesn't report back the set state of "sat"
        // Lets just assume it always succeeds
        m_sat = sat;

//        QVariantList xyList;
//        xyList << x << y;
//        params.insert("xy", xyList);


        params.insert("on", true);
        m_busyStateChangeId = HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
        m_timeout.start();
    } else {
        m_dirtyHue = hue;
        m_hueDirty = true;
        m_dirtySat = sat;
        m_satDirty = true;
//        m_xyDirty = true;
//        m_dirtyXy = QPointF(x, y);
    }
}

QPointF Group::xy() const
{
    return m_xy;
}

void Group::setXy(const QPointF &xy)
{
    if (m_xy != xy) {
        m_xy = xy;
        emit stateChanged();
    }
}

quint16 Group::ct() const
{
    return m_ct;
}

void Group::setCt(quint16 ct)
{
    if (m_busyStateChangeId == -1) {
        QVariantMap params;
        params.insert("ct", ct);
        params.insert("on", true);
        m_busyStateChangeId = HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
        m_timeout.start();
    } else {
        m_dirtyCt = ct;
        m_ctDirty = true;
    }
}

QString Group::alert() const
{
    return m_alert;
}

void Group::setAlert(const QString &alert)
{
    if (m_alert != alert) {
        QVariantMap params;
        params.insert("alert", alert);
        if (alert != "none") {
            params.insert("on", true);
        }
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
    }
}

QString Group::effect() const
{
    return m_effect;
}

void Group::setEffect(const QString &effect)
{
    if (m_effect != effect) {
        QVariantMap params;
        params.insert("effect", effect);
        if (effect != "none") {
            params.insert("on", true);
        }
        HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
    }
}

LightInterface::ColorMode Group::colorMode() const
{
    return m_colormode;
}

bool Group::reachable() const
{
    return m_reachable;
}

QList<int> Group::lightIds() const
{
    return m_lightIds;
}

bool Group::isGroup() const
{
    return true;
}

void Group::refresh()
{
    HueBridgeConnection::instance()->get("groups/" + QString::number(m_id), this, "responseReceived");
}

void Group::responseReceived(int id, const QVariant &response)
{
    Q_UNUSED(id)

    m_lightIds.clear();

    QVariantMap attributes = response.toMap();
    QVariantList lightsMap = attributes.value("lights").toList();
    foreach (const QVariant &lightId, lightsMap) {
        m_lightIds << lightId.toUInt();
    }

    emit lightsChanged();

    QVariantMap action = attributes.value("action").toMap();
    m_on = action.value("on").toBool();
    m_bri = action.value("bri").toUInt();
    m_hue = action.value("hue").toUInt();
    m_sat = action.value("sat").toUInt();

    m_xy = action.value("xy").toPointF();
    m_ct = action.value("ct").toInt();
    m_alert = action.value("alert").toString();
    m_effect = action.value("effect").toString();
    QString colorModeString = action.value("colormode").toString();
    if (colorModeString == "hs") {
        m_colormode = ColorModeHS;
    } else if (colorModeString == "xy") {
        m_colormode = ColorModeXY;
    } else if (colorModeString == "ct") {
        m_colormode = ColorModeCT;
    }
    m_reachable = true;//action.value("reachable").toBool();
    emit stateChanged();
}

void Group::setDescriptionFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "setDescription finished" << response;
    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        QVariantMap successMap = result.value("success").toMap();
        if (successMap.contains("/groups/" + QString::number(m_id) + "/name")) {
            m_name = successMap.value("/groups/" + QString::number(m_id) + "/name").toString();
            emit nameChanged();
        }
    }
}

void Group::setStateFinished(int id, const QVariant &response)
{
    qDebug() << "set state finished" << response;
    foreach (const QVariant &resultVariant, response.toList()) {
        QVariantMap result = resultVariant.toMap();
        if (result.contains("success")) {
            QVariantMap successMap = result.value("success").toMap();
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/on")) {
                m_on = successMap.value("/groups/" + QString::number(m_id) + "/action/on").toBool();
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/hue")) {
                m_hue = successMap.value("/groups/" + QString::number(m_id) + "/action/hue").toInt();
                m_colormode = ColorModeHS;
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/bri")) {
                m_bri = successMap.value("/groups/" + QString::number(m_id) + "/action/bri").toInt();
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/sat")) {
                m_sat = successMap.value("/groups/" + QString::number(m_id) + "/action/sat").toInt();
                m_colormode = ColorModeHS;
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/xy")) {
                m_xy = successMap.value("/groups/" + QString::number(m_id) + "/action/xy").toPoint();
                m_colormode = ColorModeXY;
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/ct")) {
                m_ct = successMap.value("/groups/" + QString::number(m_id) + "/action/ct").toInt();
                m_colormode = ColorModeCT;
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/effect")) {
                m_effect = successMap.value("/groups/" + QString::number(m_id) + "/action/effect").toString();
            }
            if (successMap.contains("/groups/" + QString::number(m_id) + "/action/alert")) {
                m_alert = successMap.value("/groups/" + QString::number(m_id) + "/action/alert").toString();
            }
        }
    }

    emit stateChanged();
    emit writeOperationFinished();

    if (m_busyStateChangeId == id) {
        m_busyStateChangeId = -1;
        m_timeout.stop();
        if (m_hueDirty || m_satDirty || m_briDirty) {
            QVariantMap params;
            params.insert("transitiontime", 0);
            if (m_hueDirty) {
                params.insert("hue", m_dirtyHue);
                m_hueDirty = false;
            }
            if (m_satDirty) {
                params.insert("sat", m_dirtySat);
                m_satDirty = false;
            }
            if (m_briDirty) {
                params.insert("bri", m_dirtyBri);
                m_briDirty = false;
            }

            // FIXME: There is a bug in the API that it doesn't report back the set state of "sat"
            // Lets just assume it always succeeds
            m_sat = m_dirtySat;

            m_busyStateChangeId = HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
            m_timeout.start();
        } else if(m_ctDirty) {
            QVariantMap params;
            params.insert("ct", m_dirtyCt);
            m_ctDirty = false;

            m_busyStateChangeId = HueBridgeConnection::instance()->put("groups/" + QString::number(m_id) + "/action", params, this, "setStateFinished");
            m_timeout.start();
        } else if (m_xyDirty) {
            QVariantMap params;
            QVariantList xyList;
            xyList << m_dirtyXy.x() << m_dirtyXy.y();
            params.insert("xy", xyList);
            m_xyDirty = false;

            m_busyStateChangeId = HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
            m_timeout.start();
        }
    }

}

void Group::timeout()
{
    if (m_busyStateChangeId != -1) {
        setStateFinished(m_busyStateChangeId, QVariant());
    }
}
