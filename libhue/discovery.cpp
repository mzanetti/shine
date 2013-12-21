#include "discovery.h"

#include <QDebug>

Discovery::Discovery(QObject *parent) :
    QUdpSocket(parent)
{
    //FIXME: port might be taken
    bind(QHostAddress::AnyIPv4, 1900);
    connect(this, &Discovery::readyRead, this, &Discovery::onReadyRead);

    findBridges();
}

void Discovery::findBridges()
{
    QByteArray b("M-SEARCH * HTTP/1.1\r\n"
                 "HOST: 239.255.255.250:1900\r\n"
                 "MAN: \"ssdp:discover\"\r\n"
                 "MX: 3\r\n"
                 "ST: libhue:idl\r\n");

    //FIXME: write might fail
    writeDatagram(b, QHostAddress("239.255.255.250"), 1900);
}


void Discovery::onReadyRead()
{
    while (hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        if (!m_reportedBridges.contains(sender)) {
            m_reportedBridges << sender;
            emit foundBridge(sender);
        }
    }
}
