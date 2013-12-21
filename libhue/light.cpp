#include "light.h"
#include "huebridgeconnection.h"

#include <QDebug>

Light::Light(int id, const QString &name, QObject *parent):
    QObject(parent),
    m_id(id),
    m_name(name)
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
        HueBridgeConnection::instance()->put("lights/" + QString::number(m_id), params, this, "setNameFinished");
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
        m_bri = bri;
        emit stateChanged();
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
        m_effect = effect;
        emit stateChanged();
    }
}

Light::ColorMode Light::colormode() const
{
    return m_colormode;
}

void Light::setColormode(Light::ColorMode colorMode)
{
    if (m_colormode != colorMode) {
        m_colormode = colorMode;
        emit stateChanged();
    }
}

void Light::setColorMode(const QString &colorModeString)
{
    if (colorModeString == "cs") {
        m_colormode == ColorModeCT;
    } else if (colorModeString == "hs") {
        m_colormode == ColorModeHS;
    } else if (colorModeString == "xy") {
        m_colormode == ColorModeXY;
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
    setColorMode(stateMap.value("colormode").toString());
    m_reachable = stateMap.value("reachable").toBool();
    emit stateChanged();

    qDebug() << "got light response" << m_modelId << m_type << m_swversion << m_on << m_bri << m_reachable;
}

void Light::setNameFinished(int id, const QVariant &response)
{
    qDebug() << "setName finished" << response;
}

void Light::setStateFinished(int id, const QVariant &response)
{
    qDebug() << "setState finished" << response;

    QVariantMap result = response.toList().first().toMap();

    if (result.contains("success")) {
        bool on = result.value("success").toMap().value("/lights/" + QString::number(m_id) + "/state/on").toBool();
        if (m_on != on) {
            m_on = on;
            emit stateChanged();
        }
    }
}
