#include "schedulesfiltermodel.h"

#include "schedule.h"
#include "schedules.h"

#include <QDebug>

SchedulesFilterModel::SchedulesFilterModel(QObject *parent):
    QSortFilterProxyModel(parent),
    m_hideOtherApps(false)
{


}

Schedules *SchedulesFilterModel::schedules() const
{
    return m_schedules;
}

void SchedulesFilterModel::setSchedules(Schedules *schedules)
{
    if (m_schedules != schedules) {
        m_schedules = schedules;
        setSourceModel(schedules);
        emit schedulesChanged();
    }
}

bool SchedulesFilterModel::hideOtherApps() const
{
    return m_hideOtherApps;
}

void SchedulesFilterModel::setHideOtherApps(bool hideOtherApps)
{
    if (m_hideOtherApps != hideOtherApps) {
        m_hideOtherApps = hideOtherApps;
        emit hideOtherAppsChanged();
        invalidateFilter();
    }
}

Schedule *SchedulesFilterModel::get(int row) const
{
    QModelIndex proxyIndex = index(row, 0);
    return m_schedules->get(mapToSource(proxyIndex).row());
}

bool SchedulesFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (m_hideOtherApps && !m_schedules->data(m_schedules->index(sourceRow), Schedules::RoleId).toString().startsWith("shine")) {
        return false;
    }
    return true;
}
