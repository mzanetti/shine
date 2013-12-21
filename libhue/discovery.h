#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <QUdpSocket>
#include <QHostAddress>

class QTimer;

class Discovery: public QUdpSocket
{
    Q_OBJECT

public:
    Discovery(QObject *parent);
    void findBridges();

signals:
    void error();
    void foundBridge(QHostAddress bridge);
    void noBridgesFound();

private slots:
    void onTimeout();
    void onReadyRead();

private:
    QList<QHostAddress> m_reportedBridges;
    QTimer *m_timeout;
};

#endif
