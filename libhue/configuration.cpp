#include "configuration.h"
#include "huebridgeconnection.h"

#include <QNetworkAccessManager>
#include <QDebug>

Configuration::Configuration(QObject *parent)
{
    connect(HueBridgeConnection::instance(), &HueBridgeConnection::usernameChanged, this, &Configuration::refresh);
}

void Configuration::refresh()
{
    HueBridgeConnection::instance()->get("config", this, "responseReceived");
}

QString Configuration::name()
{
    return m_name;
}

void Configuration::setName(const QString &name)
{
    if (m_name == name) {
        return;
    }
}

void Configuration::responseReceived(int id, const QVariantMap &data)
{
    qDebug() << "got config response" << data;
}
