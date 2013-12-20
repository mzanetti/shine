#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>

class Light: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    Light(int id, QObject *parent = 0);

    int id() const;

    QString name() const;
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    int m_id;
    QString m_name;
};

#endif
