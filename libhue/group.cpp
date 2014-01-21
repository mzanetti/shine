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

Group::Group(int id, const QString &name, QObject *parent)
    : LightInterface(parent)
    , m_id(id)
    , m_name(name)
{
    refresh();
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
    quint8 bri = 0;
    foreach (int lightId, m_lightIds) {
//        if (bri > 0 && Lights::get(lightId)->bri() != bri)
//            return 0;
//        bri = Lights::get(lightId)->bri();
    }

    return bri;
}

void Group::setBri(quint8 bri)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setBri(bri);
    }

    emit stateChanged();
}

quint16 Group::hue() const
{
    quint16 hue = 0;
    foreach (int lightId, m_lightIds) {
        //        if (hue > 0 && Lights::get(lightId)->hue() != hue)
        //            return 0;
        //        hue = Lights::get(lightId)->hue();
    }

    return hue;
}

void Group::setHue(quint16 hue)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setHue(hue);
    }

    emit stateChanged();
}

quint8 Group::sat() const
{
    quint8 sat = 0;
    foreach (int lightId, m_lightIds) {
        //        if (sat > 0 && Lights::get(lightId)->sat() != sat)
        //            return 0;
        //        sat = Lights::get(lightId)->sat();
    }

    return sat;
}

void Group::setSat(quint8 sat)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setSat(sat);
    }

    emit stateChanged();
}

QColor Group::color() const
{
    return QColor::fromHsv(hue() * 360 / 65535, sat(), 255);
}

void Group::setColor(const QColor &color)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setColor(color);
    }

    emit stateChanged();
}

QPointF Group::xy() const
{
    QPointF p;
    foreach (int lightId, m_lightIds) {
        //        if ((p.x() > 0 || p.y() > 0) && Lights::get(lightId)->xy() != p)
        //            return QPointF();
        //        p = Lights::get(lightId)->xy();
    }

    return p;
}

void Group::setXy(const QPointF &xy)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setXy(xy);
    }

    emit stateChanged();
}

quint16 Group::ct() const
{
    quint16 ct = 0;
    foreach (int lightId, m_lightIds) {
        //        if (ct > 0 && Lights::get(lightId)->ct() != ct)
        //            return 0;
        //        ct = Lights::get(lightId)->ct();
    }

    return ct;
}

void Group::setCt(quint16 ct)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setCt(ct);
    }

    emit stateChanged();
}

QString Group::alert() const
{
    QString alert;
    foreach (int lightId, m_lightIds) {
        //        if (!alert.isEmpty() && Lights::get(lightId)->alert() != alert)
        //            return QString();
        //        alert = Lights::get(lightId)->alert();
    }

    return alert;
}

void Group::setAlert(const QString &alert)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setAlert(alert);
    }

    emit stateChanged();
}

QString Group::effect() const
{
    QString effect;
    foreach (int lightId, m_lightIds) {
        //        if (!effect.isEmpty() && Lights::get(lightId)->effect() != effect)
        //            return QString();
        //        effect = Lights::get(lightId)->effect();
    }

    return effect;
}

void Group::setEffect(const QString &effect)
{
    foreach (int lightId, m_lightIds) {
        //        Lights::get(lightId)->setEffect(effect);
    }

    emit stateChanged();
}

LightInterface::ColorMode Group::colorMode() const
{
    ColorMode colormode = ColorModeHS;
    foreach (int lightId, m_lightIds) {
        //        if (!colormode.isEmpty() && Lights::get(lightId)->colormode() != colormode)
        //            return QString();
        //        colormode = Lights::get(lightId)->colormode();
    }

    return colormode;
}

bool Group::reachable() const
{
    foreach (int lightId, m_lightIds) {
        //        if (Lights::get(lightId)->reachable())
        //            return true;
    }

    return false;
}

QList<int> Group::lightIds() const
{
    return m_lightIds;
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
    foreach (const QVariant &resultVariant, response.toList()) {
        QVariantMap result = resultVariant.toMap();
        if (result.contains("success")) {
            QVariantMap successMap = result.value("success").toMap();
            if (successMap.contains("/groups/" + QString::number(m_id) + "/state/on")) {
                m_on = successMap.value("/groups/" + QString::number(m_id) + "/state/on").toBool();
            }
        }
    }
    emit stateChanged();
}
