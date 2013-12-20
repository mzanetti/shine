#ifndef LIGHTS_H
#define LIGHTS_H

#include <QAbstractListModel>

class Light;

class Lights : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId,
        RoleName,
        RoleOn,
        RoleSaturation,
        RoleBrightness,
        RoleHue
    };

    explicit Lights(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void refresh();

private slots:
    void lightsReceived(int id, const QVariant &variant);

private:
    QList<Light*> m_list;
};

#endif // LIGHTS_H
