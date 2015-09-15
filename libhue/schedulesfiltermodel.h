#ifndef SCHEDULESFILTERMODEL_H
#define SCHEDULESFILTERMODEL_H

#include <QSortFilterProxyModel>

class Schedule;
class Schedules;

class SchedulesFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Schedules* schedules READ schedules WRITE setSchedules NOTIFY schedulesChanged)
    Q_PROPERTY(bool hideOtherApps READ hideOtherApps WRITE setHideOtherApps NOTIFY hideOtherAppsChanged)

public:
    SchedulesFilterModel(QObject *parent = 0);

    Schedules* schedules() const;
    void setSchedules(Schedules *schedules);

    bool hideOtherApps() const;
    void setHideOtherApps(bool hideOtherApps);

    Q_INVOKABLE Schedule* get(int row) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void schedulesChanged();
    void hideOtherAppsChanged();

private:
    Schedules *m_schedules;
    bool m_hideOtherApps;
};

#endif
