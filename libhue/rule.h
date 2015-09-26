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

#ifndef RULE_H
#define RULE_H

#include <QObject>
#include <QAbstractListModel>

class Rule: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariantList conditions READ conditions NOTIFY conditionsChanged)
    Q_PROPERTY(QVariantList actions READ actions NOTIFY actionsChanged)

public:
    Rule(const QString &id, const QString &name, QObject *parent = 0);

    QString id() const;

    QString name() const;
    void setName(const QString &name);

    QVariantList conditions() const;
    void setConditions(const QVariantList &conditions);

    QVariantList actions() const;
    void setActions(const QVariantList &actions);

public slots:
    void refresh();

signals:
    void nameChanged();
    void conditionsChanged();
    void actionsChanged();

private:
    QString m_id;
    QString m_name;
    QVariantList m_conditions;
    QVariantList m_actions;
};

#endif
