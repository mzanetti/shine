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

#include "discovery.h"

#include <QDebug>
#include <QTimer>

// Discovery timeout in seconds
const unsigned int DISCOVERY_TIMEOUT = 3;

Discovery::Discovery(QObject *parent) :
    QUdpSocket(parent),
    m_timeout(new QTimer(this))
{
    quint16 port = 1900;
    unsigned int tries = 0;
    const unsigned int maxtries = 10;

    while (!bind(port++)) {
        if (++tries == maxtries) {
            QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection);
            return;
        }
    }

    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    m_timeout->setSingleShot(true);
    connect(m_timeout, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

void Discovery::findBridges()
{
    m_timeout->stop();
    m_reportedBridges.clear();

    QString b("M-SEARCH * HTTP/1.1\r\n"
              "HOST: 239.255.255.250:1900\r\n"
              "MAN: \"ssdp:discover\"\r\n"
              "MX: %1\r\n"
              "ST: libhue:idl\r\n");
    b.arg(DISCOVERY_TIMEOUT);

//    qDebug() << "writing datagram" << b;
    m_timeout->start(DISCOVERY_TIMEOUT * 1000);
    if (writeDatagram(b.toUtf8(), QHostAddress("239.255.255.250"), 1900) < 0) {
        qDebug() << errorString();
        emit error();
    }
}

void Discovery::onTimeout()
{
    if (m_reportedBridges.isEmpty()) {
        emit noBridgesFound();
        // Try again...
        findBridges();
    }
}

void Discovery::onReadyRead()
{
    while (hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

//        qDebug() << "got datagram" << datagram;
        sender.setAddress(sender.toIPv4Address());
        if (!m_reportedBridges.contains(sender)) {
            m_reportedBridges << sender;
            emit foundBridge(sender);
        }
    }
}
