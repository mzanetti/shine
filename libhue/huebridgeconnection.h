#ifndef HUEBRIDGECONNECTION_H
#define HUEBRIDGECONNECTION_H

#include <QObject>
#include <QHash>
#include <QHostAddress>

class QNetworkAccessManager;
class QNetworkReply;

class CallbackObject
{
public:
    CallbackObject(QObject *sender = 0, const QString &slot = QString()):
        m_sender(sender),
        m_slot(slot)
    {}
    QObject *sender() const { return m_sender; }
    QString slot() const { return m_slot; }
private:
    QObject *m_sender;
    QString m_slot;
};

class HueBridgeConnection: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)

public:
    static HueBridgeConnection* instance();

    QString username() const;
    void setUsername(const QString &username);
    Q_INVOKABLE void createUser(const QString &devicetype, const QString &username);

    int get(const QString &path, QObject *sender, const QString &slot);
    int post(const QString &path, const QVariantMap &params, QObject *sender, const QString &slot);

signals:
    void usernameChanged();

    void getFinished(int id, const QVariantMap &result);
    void postFinished(int id, const QVariantMap &result);

private slots:
    void onFoundBridge(QHostAddress bridge);
    void createUserFinished();
    void slotGetFinished();

private:
    HueBridgeConnection();
    static HueBridgeConnection *s_instance;

    QNetworkAccessManager *m_nam;
    QString m_username;

    int m_requestCounter;
    QHash<QNetworkReply*, int> m_requestIdMap;
    QHash<int, CallbackObject> m_requestSenderMap;

    QHostAddress m_bridge;
};

#endif
