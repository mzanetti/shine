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
        RoleModelId,
        RoleType,
        RoleSwVersion,
        RoleOn,
        RoleBrightness,
        RoleHue,
        RoleSaturation,
        RoleXY,
        RoleAlert,
        RoleEffect,
        RoleColorMode,
        RoleReachable
    };

    explicit Lights(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void refresh();

private slots:
    void lightsReceived(int id, const QVariant &variant);
    void lightDescriptionChanged();
    void lightStateChanged();

private:
    Light* createLight(int id, const QString &name);

private:
    QList<Light*> m_list;
};

#endif // LIGHTS_H
