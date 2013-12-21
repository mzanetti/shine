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

#include "light.h"
#include "huebridgeconnection.h"

#include <QColor>
#include <QDebug>

Light::Light(int id, const QString &name, QObject *parent):
    QObject(parent),
    m_id(id),
    m_name(name),
    m_setColorId(-1)
{
    HueBridgeConnection::instance()->get("lights/" + QString::number(id), this, "responseReceived");

}

int Light::id() const
{
    return m_id;
}

QString Light::name() const
{
    return m_name;
}

void Light::setName(const QString &name)
{
    if (m_name != name) {
        QVariantMap params;
        params.insert("name", name);
        HueBridgeConnection::instance()->put("lights/" + QString::number(m_id), params, this, "setDescriptionFinished");
    }
}

QString Light::modelId() const
{
    return m_modelId;
}

void Light::setModelId(const QString &modelId)
{
    if (m_modelId != modelId) {
        m_modelId = modelId;
        emit modelIdChanged();
    }
}

QString Light::type() const
{
    return m_type;
}

void Light::setType(const QString &type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString Light::swversion() const
{
    return m_swversion;
}

void Light::setSwversion(const QString &swversion)
{
    if (m_swversion != swversion) {
        m_swversion = swversion;
        emit swversionChanged();
    }

}

bool Light::on() const
{
    return m_on;
}

void Light::setOn(bool on)
{
    if (m_on != on) {
        QVariantMap params;
        params.insert("on", on);
        HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
    }

}

quint8 Light::bri() const
{
    return m_bri;
}

void Light::setBri(quint8 bri)
{
    if (m_bri != bri) {
        QVariantMap params;
        params.insert("bri", bri);
        HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
    }
}

quint16 Light::hue() const
{
    return m_hue;
}

void Light::setHue(quint16 hue)
{
    if (m_hue != hue) {
        m_hue = hue;
        emit stateChanged();
    }
}

quint8 Light::sat() const
{
    return m_sat;
}

void Light::setSat(quint8 sat)
{
    if (m_sat != sat) {
        m_sat = sat;
        emit stateChanged();
    }
}

QColor Light::color() const
{
    return QColor::fromHsv(m_hue * 360 / 65535, m_sat, 255);
}

void Light::setColor(const QColor &color)
{
    quint16 hue = color.hue() * 65535 / 360;
    quint8 sat = color.saturation();

    QVariantMap params;
    params.insert("hue", hue);
    params.insert("sat", sat);
    if (m_setColorId == -1) {
        m_setColorId = HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
    } else {
        m_dirtyHue = hue;
        m_dirtySat = sat;
        m_outOfSync = true;
    }
}

QPointF Light::xy() const
{
    return m_xy;
}

void Light::setXy(const QPointF &xy)
{
    if (m_xy != xy) {
        m_xy = xy;
        emit stateChanged();
    }
}

quint16 Light::ct() const
{
    return m_ct;
}

void Light::setCt(quint16 ct)
{
    if (m_ct != ct) {
        m_ct = ct;
        emit stateChanged();
    }
}

QString Light::alert() const
{
    return m_alert;
}

void Light::setAlert(const QString &alert)
{
    if (m_alert != alert) {
        m_alert = alert;
        emit stateChanged();
    }
}

QString Light::effect() const
{
    return m_effect;
}

void Light::setEffect(const QString &effect)
{
    if (m_effect != effect) {
        QVariantMap params;
        params.insert("effect", effect);
        HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
    }
}

QString Light::colormode() const
{
    return m_colormode;
}

void Light::setColormode(const QString &colorMode)
{
    if (m_colormode != colorMode) {
        m_colormode = colorMode;
        emit stateChanged();
    }
}

bool Light::reachable() const
{
    return m_reachable;
}

void Light::setReachable(bool reachable)
{
    if (m_reachable != reachable) {
        m_reachable = reachable;
        emit stateChanged();
    }
}

void Light::responseReceived(int id, const QVariant &response)
{
    Q_UNUSED(id)
    QVariantMap attributes = response.toMap();

    setModelId(attributes.value("modelid").toString());
    setType(attributes.value("type").toString());
    setSwversion(attributes.value("swversion").toString());

    QVariantMap stateMap = attributes.value("state").toMap();
    m_on = stateMap.value("on").toBool();
    m_bri = stateMap.value("bri").toInt();
    m_hue = stateMap.value("hue").toInt();
    m_sat = stateMap.value("sat").toInt();
    m_xy = stateMap.value("xy").toPointF();
    m_ct = stateMap.value("ct").toInt();
    m_alert = stateMap.value("alert").toString();
    m_effect = stateMap.value("effect").toString();
    m_colormode = stateMap.value("colormode").toString();
    m_reachable = stateMap.value("reachable").toBool();
    emit stateChanged();

    qDebug() << "got light response" << m_modelId << m_type << m_swversion << m_on << m_bri << m_reachable;
}

void Light::setDescriptionFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "setDescription finished" << response;
    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        QVariantMap successMap = result.value("success").toMap();
        if (successMap.contains("/lights/" + QString::number(m_id) + "/name")) {
            m_name = successMap.value("/lights/" + QString::number(m_id) + "/name").toString();
            emit nameChanged();
        }
    }
}

void Light::setStateFinished(int id, const QVariant &response)
{
    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        QVariantMap successMap = result.value("success").toMap();
        if (successMap.contains("/lights/" + QString::number(m_id) + "/state/on")) {
            m_on = successMap.value("/lights/" + QString::number(m_id) + "/state/on").toBool();
        }
        if (successMap.contains("/lights/" + QString::number(m_id) + "/state/bri")) {
            m_bri = successMap.value("/lights/" + QString::number(m_id) + "/state/bri").toInt();
        }
        if (successMap.contains("/lights/" + QString::number(m_id) + "/state/sat")) {
            m_sat = successMap.value("/lights/" + QString::number(m_id) + "/state/sat").toInt();
        }
        if (successMap.contains("/lights/" + QString::number(m_id) + "/state/effect")) {
            m_effect = successMap.value("/lights/" + QString::number(m_id) + "/state/effect").toString();
        }

        emit stateChanged();
    }

    if (m_setColorId == id) {
        m_setColorId = -1;
        if (m_outOfSync) {
            QVariantMap params;
            params.insert("hue", m_dirtyHue);
            params.insert("sat", m_dirtySat);
            m_setColorId = HueBridgeConnection::instance()->put("lights/" + QString::number(m_id) + "/state", params, this, "setStateFinished");
            m_outOfSync = false;
        }
    }
}
