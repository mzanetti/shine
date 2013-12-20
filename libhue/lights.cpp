#include "lights.h"

Lights::Lights(QObject *parent) :
    QAbstractListModel(parent)
{
}

int Lights::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant Lights::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QHash<int, QByteArray> Lights::roles() const
{
    QHash<int, QByteArray> roles;
    return roles;
}
