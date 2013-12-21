#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <QUdpSocket>
#include <QHostAddress>

class Discovery: public QUdpSocket
{
    Q_OBJECT

public:
    Discovery(QObject *parent);
    void findBridges();

signals:
    void foundBridge(QHostAddress bridge);

private slots:
    void onReadyRead();

private:
    QList<QHostAddress> m_reportedBridges;
};

#endif
