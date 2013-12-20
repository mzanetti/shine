#ifndef CONFIGRATION_H
#define CONFIGURATION_H

#include <QObject>

class Configuration: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    Configuration(QObject *parent = 0);

    void refresh();

    QString name();
    void setName(const QString &name);

signals:
    void nameChanged();

private slots:
    void responseReceived(int id, const QVariantMap &data);

private:
    QString m_name;
};

#endif
