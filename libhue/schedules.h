/*
 * Copyright 2015 Michael Zanetti
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Michael Zanetti <michael_zanetti@gmx.net>
 */

#ifndef SCHEDULES_H
#define SCHEDULES_H

#include "huemodel.h"

#include <QTimer>
class Schedule;

class Schedules: public HueModel
{
    Q_OBJECT
public:
    enum Roles {
        RoleId,
        RoleName,
        RoleDateTime,
        RoleType,
        RoleRecurring,
        RoleWeekdays
    };

    explicit Schedules(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Schedule* get(int index) const;
    Q_INVOKABLE Schedule* findSchedule(const QString &id) const;

    bool busy() const;

public slots:
    Q_INVOKABLE void createSingleAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &dateTime);
    Q_INVOKABLE void createRecurringAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &time, const QString &weekdays);
    Q_INVOKABLE void createSingleAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime);
    Q_INVOKABLE void createRecurringAlarmForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays);
    Q_INVOKABLE void createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &dateTime);
    Q_INVOKABLE void createSingleAlarmForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &time, const QString &weekdays);

    Q_INVOKABLE void createTimerForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat = -1);
    Q_INVOKABLE void createTimerForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QDateTime &timeFromNow, int repeat = -1);

    Q_INVOKABLE void deleteSchedule(const QString &id);

    void refresh();

signals:
    void countChanged();

private slots:
    void createAlarmForScene(const QString &name, const QString &sceneId, const QString &timeString);
    void createScheduleForLight(const QString &name, int lightId, bool on, quint8 bri, const QColor &color, const QString &timeString);
    void createScheduleForGroup(const QString &name, int groupId, bool on, quint8 bri, const QColor &color, const QString &timeString);
    void createSchedule(const QString &name, const QVariantMap &command, const QString &timeString);

    void createScheduleFinished(int id, const QVariant &variant);
    void deleteScheduleFinished(int id, const QVariant &variant);
    void schedulesReceived(int id, const QVariant &variant);

private:
    Schedule* createScheduleInternal(const QString &id, const QString &name);

    QList<Schedule*> m_list;
    bool m_busy;
};

#endif
