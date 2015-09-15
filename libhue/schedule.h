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

#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QAbstractListModel>
#include <QDateTime>

class Schedule: public QObject
{
    Q_OBJECT

    Q_ENUMS(Type)
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(QDateTime dateTime READ dateTime NOTIFY dateTimeChanged)
    Q_PROPERTY(QString weekdays READ weekdays NOTIFY weekdaysChanged)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(bool autodelete READ autodelete NOTIFY autodeleteChanged)
    Q_PROPERTY(bool recurring READ recurring NOTIFY recurringChanged)

public:
    enum Type {
        TypeAlarm,
        TypeTimer
    };

    Schedule(const QString &id, const QString &name, QObject *parent = 0);

    QString id() const;

    QString name() const;
    void setName(const QString &name);

    Type type() const;
    void setType(Type type);

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    QString weekdays() const;
    void setWeekdays(const QString &weekdays);

    bool enabled() const;
    void setEnabled(bool enabled);

    bool autodelete() const;
    void setAutoDelete(bool autodelete);

    bool recurring() const;
    void setRecurring(bool recurring);

public slots:
    void refresh();

signals:
    void nameChanged();
    void typeChanged();
    void dateTimeChanged();
    void weekdaysChanged();
    void enabledChanged();
    void autodeleteChanged();
    void recurringChanged();

private slots:
//    void responseReceived(int id, const QVariant &response);
//    void setDescriptionFinished(int id, const QVariant &response);

//    void setStateFinished(int id, const QVariant &response);

private:
    QString m_id;
    QString m_name;
    Type m_type;
    QDateTime m_dateTime;
    QString m_weekdays;
    bool m_enabled;
    bool m_autodelete;
    bool m_recurring;
};

#endif
