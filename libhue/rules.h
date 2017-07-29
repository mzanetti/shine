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

#ifndef RULES_H
#define RULES_H

#include "huemodel.h"

#include <QTimer>

class Rule;

class Rules: public HueModel
{
    Q_OBJECT
    Q_ENUMS(DimAction)
public:
    enum Roles {
        RoleId = Qt::UserRole,
        RoleName,
        RoleConditions,
        RoleActions
    };

    enum DimAction {
        DimActionUp,
        DimActionDown
    };

    explicit Rules(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE Rule* get(int index) const;
    Q_INVOKABLE Rule* findRule(const QString &id) const;
    Q_INVOKABLE void deleteRule(int ruleId);

    Q_INVOKABLE void createRule(const QString &name, const QVariantList &conditions, const QVariantList &actions);

    Q_INVOKABLE QVariantMap createHelperCondition(int helperSensorId, const QString &op, const QString &value);
    Q_INVOKABLE QVariantList createHueTapConditions(int tapSensorId, int buttonId);
    Q_INVOKABLE QVariantList createHueDimmerConditions(int dimmerSensorId, int buttonId);
    Q_INVOKABLE QVariantList createDaylightConditions(int daylightSensorId, bool day);

    Q_INVOKABLE QVariantMap createHelperAction(int helperSensorId, int value);
    Q_INVOKABLE QVariantMap createLightAction(int lightId, bool on);
    Q_INVOKABLE QVariantMap createLightDimmerAction(int lightId, DimAction dimAction);
    Q_INVOKABLE QVariantMap createLightColorAction(int lightId, const QColor &color, int bri);
    Q_INVOKABLE QVariantMap createGroupAction(int groupId, bool on);
    Q_INVOKABLE QVariantMap createGroupDimmerAction(int lightId, DimAction dimAction);
    Q_INVOKABLE QVariantMap createGroupColorAction(int groupId, const QColor &color, int bri);
    Q_INVOKABLE QVariantMap createSceneAction(const QString &sceneId);
    Q_INVOKABLE QVariantList createLightTimerActions(int lightId);

    bool busy() const;

public slots:
    void refresh();

private slots:
    void rulesReceived(int id, const QVariant &variant);

    void ruleDeleted(int, const QVariant &response);
    void createRuleFinished(int id, const QVariant &response);

private:
    Rule* createRuleInternal(const QString &id, const QString &name);

    QList<Rule*> m_list;
    bool m_busy;
};

#endif
