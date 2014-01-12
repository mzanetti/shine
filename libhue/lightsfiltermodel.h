#ifndef LIGHTSFILTERMODEL_H
#define LIGHTSFILTERMODEL_H

#include <QSortFilterProxyModel>

class Group;
class Groups;
class Lights;
class Light;

class LightsFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int groupId READ groupId WRITE setGroupId NOTIFY groupIdChanged)
    Q_PROPERTY(Lights* lights READ lights WRITE setLights NOTIFY lightsChanged)

public:
    LightsFilterModel(QObject *parent = 0);

    int groupId() const;
    void setGroupId(int groupId);

    Lights* lights() const;
    void setLights(Lights *lights);

    Q_INVOKABLE Light* get(int row) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void groupIdChanged();
    void lightsChanged();

private slots:
    void groupChanged(const QModelIndex &first, const QModelIndex &last, const QVector<int> &roles);
    void groupsAdded(const QModelIndex &parent, int first, int last);
    void groupsReset();

private:
    Group* findGroup();

private:
    quint16 m_groupId;
    Group *m_group;
    Groups *m_groups;
    Lights *m_lights;
};

#endif
