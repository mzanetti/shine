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

#include <QAbstractListModel>

class Schedule;

class Schedules: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        RoleId,
        RoleName,
        RoleDateTime,
        RoleRecurring,
        RoleWeekdays
    };

    explicit Schedules(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int count() const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Schedule* get(int index) const;
    Q_INVOKABLE Schedule* findSchedule(const QString &id) const;

//    Q_INVOKABLE void recallScene(int index);

public slots:
    Q_INVOKABLE void createSingleAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &dateTime);
    Q_INVOKABLE void createRecurringAlarmForScene(const QString &name, const QString &sceneId, const QDateTime &time, const QString &weekdays);
    Q_INVOKABLE void createSchedule(const QString &name, const QList<int> &lights);
    Q_INVOKABLE void updateSchedule(const QString &id, const QString &name, const QList<int> &lights);
    Q_INVOKABLE void deleteSchedule(const QString &id);

    void refresh();

signals:
    void countChanged();

private slots:
    void createAlarmForScene(const QString &name, const QString &sceneId, const QString &timeString);
    void createScheduleFinished(int id, const QVariant &variant);
//    void recallSceneFinished(int id, const QVariant &variant);
    void deleteScheduleFinished(int id, const QVariant &variant);
    void schedulesReceived(int id, const QVariant &variant);
//    void sceneNameChanged();
////    void groupLightsChanged();

private:
    Schedule* createScheduleInternal(const QString &id, const QString &name);

    QList<Schedule*> m_list;
};

#endif // SCENES_H
