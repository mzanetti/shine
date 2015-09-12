#include "scenesfiltermodel.h"

#include "scene.h"
#include "scenes.h"

#include <QDebug>

ScenesFilterModel::ScenesFilterModel(QObject *parent):
    QSortFilterProxyModel(parent),
    m_hideOtherApps(false)
{


}

Scenes *ScenesFilterModel::scenes() const
{
    return m_scenes;
}

void ScenesFilterModel::setScenes(Scenes *scenes)
{
    if (m_scenes != scenes) {
        m_scenes = scenes;
        setSourceModel(scenes);
        emit scenesChanged();
    }
}

bool ScenesFilterModel::hideOtherApps() const
{
    return m_hideOtherApps;
}

void ScenesFilterModel::setHideOtherApps(bool hideOtherApps)
{
    if (m_hideOtherApps != hideOtherApps) {
        m_hideOtherApps = hideOtherApps;
        emit hideOtherAppsChanged();
        invalidateFilter();
    }
}

Scene *ScenesFilterModel::get(int row) const
{
    QModelIndex proxyIndex = index(row, 0);
    return m_scenes->get(mapToSource(proxyIndex).row());
}

bool ScenesFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (m_hideOtherApps && !m_scenes->data(m_scenes->index(sourceRow), Scenes::RoleId).toString().startsWith("shine")) {
        return false;
    }
    return true;
}
