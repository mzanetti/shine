#include "lightsfiltermodel.h"

#include "groups.h"
#include "group.h"
#include "lights.h"
#include "light.h"

#include <QDebug>

LightsFilterModel::LightsFilterModel(QObject *parent):
    QSortFilterProxyModel(parent),
    m_groupId(0),
    m_groups(0) // Only creating when we need it to avoid network calls
{


}

int LightsFilterModel::groupId() const
{
    return m_groupId;
}

void LightsFilterModel::setGroupId(int groupId)
{
    qDebug() << Q_FUNC_INFO;
    if (m_groupId != groupId) {
        m_groupId = groupId;


        if (!m_groups) {
            m_groups = new Groups(this);
            connect(m_groups, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(groupChanged(QModelIndex,QModelIndex,QVector<int>)));
            connect(m_groups, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(groupsAdded(QModelIndex,int,int)));
            connect(m_groups, SIGNAL(modelReset()), this, SLOT(groupsReset()));
        } else {
            m_groups->refresh();
        }
        m_group = findGroup();
        invalidateFilter();

        emit groupIdChanged();
    }
}

Lights *LightsFilterModel::lights() const
{
    return m_lights;
}

void LightsFilterModel::setLights(Lights *lights)
{
    if (m_lights != lights) {
        m_lights = lights;
        setSourceModel(lights);
        emit lightsChanged();
    }
}

Light *LightsFilterModel::get(int row) const
{
    QModelIndex proxyIndex = index(row, 0);
    return m_lights->get(mapToSource(proxyIndex).row());
}

bool LightsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_groupId == 0) {
        return true;
    }

    Light *light = m_lights->get(sourceRow);
    if (m_group && m_group->lightIds().contains(light->id())) {
        return true;
    }

    return false;
}

void LightsFilterModel::groupChanged(const QModelIndex &first, const QModelIndex &last, const QVector<int> &roles)
{
    if (!roles.contains(Groups::RoleLightIds)) {
        return;
    }

    for (int i = first.row(); i <= last.row(); ++i) {
        if (m_group == m_groups->get(i)) {
            invalidateFilter();
        }
    }
}

void LightsFilterModel::groupsAdded(const QModelIndex &parent, int first, int last)
{
    if (!m_group) {
        m_group = findGroup();
    }
    invalidateFilter();
}

void LightsFilterModel::groupsReset()
{
    m_group = findGroup();
    invalidateFilter();
}

Group *LightsFilterModel::findGroup()
{
    for (int i = 0; i < m_groups->rowCount(QModelIndex()); ++i) {
        Group *group = m_groups->get(i);
        if (group->id() == m_groupId) {
            return group;
        }
    }
    return 0;
}
