#ifndef LIGHTS_H
#define LIGHTS_H

#include <QAbstractListModel>

class Lights : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleOn,
        RoleSaturation,
        RoleBrightness,
        RoleHue
    };

    explicit Lights(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roles() const;

signals:

public slots:

};

#endif // LIGHTS_H
