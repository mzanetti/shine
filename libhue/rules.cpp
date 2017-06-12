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

#include "rules.h"
#include "rule.h"

#include "huebridgeconnection.h"

#include <QDebug>
#include <QUuid>
#include <QColor>
#include <QTime>

Rules::Rules(QObject *parent):
    HueModel(parent),
    m_busy(false)
{
#if QT_VERSION < 0x050000
    setRoleNames(roleNames());
#endif
}

int Rules::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_list.count();
}

QVariant Rules::data(const QModelIndex &index, int role) const
{
    Rule *rule = m_list.at(index.row());
    switch (role) {
    case RoleId:
        return rule->id();
    case RoleName:
        return rule->name();
    case RoleConditions:
        return rule->conditions();
    case RoleActions:
        return rule->actions();
    }

    return QVariant();
}

QHash<int, QByteArray> Rules::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(RoleId, "id");
    roles.insert(RoleName, "name");
    roles.insert(RoleConditions, "conditions");
    roles.insert(RoleActions, "actions");
    return roles;
}

Rule *Rules::get(int index) const
{
    if (index > -1 && index  < m_list.count()) {
        return m_list.at(index);
    }
    return 0;
}

Rule* Rules::findRule(const QString &id) const
{
    foreach (Rule *rule, m_list) {
        if (rule->id() == id) {
            return rule;
        }
    }
    return 0;
}

void Rules::deleteRule(int ruleId)
{
    HueBridgeConnection::instance()->deleteResource("rules/" + QString::number(ruleId), this, "ruleDeleted");
}

void Rules::createRule(const QString &name, const QVariantList &conditions, const QVariantList &actions)
{
    QVariantMap params;
    params.insert("name", name);
    params.insert("status", "enabled");
    params.insert("conditions", conditions);
    params.insert("actions", actions);
    HueBridgeConnection::instance()->post("rules", params, this, "createRuleFinished");
}

QVariantMap Rules::createHelperCondition(int helperSensorId, const QString &op, const QString &value)
{
    QVariantMap condition;
    condition.insert("address", "/sensors/" + QString::number(helperSensorId) + "/state/status");
    condition.insert("operator", op);
    condition.insert("value", value);
    return condition;
}

QVariantList Rules::createHueTapConditions(int tapSensorId, int buttonId)
{
    QVariantList conditions;
    QVariantMap buttonIdCondition;
    buttonIdCondition.insert("address", "/sensors/" + QString::number(tapSensorId) + "/state/buttonevent");
    buttonIdCondition.insert("operator", "eq");
    buttonIdCondition.insert("value", QString::number(buttonId));
    conditions.append(buttonIdCondition);

    QVariantMap buttonPressedCondition;
    buttonPressedCondition.insert("address", "/sensors/" + QString::number(tapSensorId) + "/state/lastupdated");
    buttonPressedCondition.insert("operator", "dx");
    conditions.append(buttonPressedCondition);

    return conditions;
}

QVariantList Rules::createHueDimmerConditions(int dimmerSensorId, int buttonId)
{
    QVariantList conditions;
    QVariantMap buttonIdCondition;
    buttonIdCondition.insert("address", "/sensors/" + QString::number(dimmerSensorId) + "/state/buttonevent");
    buttonIdCondition.insert("operator", "eq");
    buttonIdCondition.insert("value", QString::number(buttonId));
    conditions.append(buttonIdCondition);

    QVariantMap buttonPressedCondition;
    buttonPressedCondition.insert("address", "/sensors/" + QString::number(dimmerSensorId) + "/state/lastupdated");
    buttonPressedCondition.insert("operator", "dx");
    conditions.append(buttonPressedCondition);

    return conditions;
}

QVariantList Rules::createDaylightConditions(int daylightSensorId, bool day)
{
    QVariantList conditions;
    QVariantMap daylightCondition;
    daylightCondition.insert("address", "/sensors/" + QString::number(daylightSensorId) + "/state/daylight");
    daylightCondition.insert("operator", "eq");
    daylightCondition.insert("value", day);
    conditions.append(daylightCondition);

    return conditions;
}

QVariantMap Rules::createHelperAction(int helperSensorId, int value)
{
    QVariantMap action;
    action.insert("address", "/sensors/" + QString::number(helperSensorId) + "/state");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("status", value);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createLightAction(int lightId, bool on)
{
    QVariantMap action;
    action.insert("address", "/lights/" + QString::number(lightId) + "/state");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", on);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createLightDimmerAction(int lightId, Rules::DimAction dimAction)
{
    QVariantMap action;
    action.insert("address", "/lights/" + QString::number(lightId) + "/state");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", true);
    body.insert("bri_inc", dimAction == DimActionUp ? 30 : -30);
    body.insert("transitiontime", 9);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createLightColorAction(int lightId, const QColor &color, int bri)
{
    // Transform from RGB to Hue/Sat
    quint16 hue = color.hue() * 65535 / 360;
    quint8 sat = color.saturation();

    QVariantMap action;
    action.insert("address", "/lights/" + QString::number(lightId) + "/state");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", true);
    body.insert("hue", hue);
    body.insert("sat", sat);
    body.insert("bri", bri);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createGroupAction(int groupId, bool on)
{
    QVariantMap action;
    action.insert("address", "/groups/" + QString::number(groupId) + "/action");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", on);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createGroupDimmerAction(int lightId, Rules::DimAction dimAction)
{
    QVariantMap action;
    action.insert("address", "/groups/" + QString::number(lightId) + "/action");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", true);
    body.insert("bri_inc", dimAction == DimActionUp ? 30 : -30);
    body.insert("transitiontime", 9);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createGroupColorAction(int groupId, const QColor &color, int bri)
{
    // Transform from RGB to Hue/Sat
    quint16 hue = color.hue() * 65535 / 360;
    quint8 sat = color.saturation();

    QVariantMap action;
    action.insert("address", "/groups/" + QString::number(groupId) + "/action");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("on", true);
    body.insert("hue", hue);
    body.insert("sat", sat);
    body.insert("bri", bri);
    action.insert("body", body);
    return action;
}

QVariantMap Rules::createSceneAction(const QString &sceneId)
{
    QVariantMap action;
    action.insert("address", "/groups/0/action");
    action.insert("method", "PUT");
    QVariantMap body;
    body.insert("scene", sceneId);
    action.insert("body", body);
    return action;
}

QVariantList Rules::createLightTimerActions(int lightId)
{
    QVariantList actions;
    QVariantMap selectAction;
    selectAction.insert("address", "/lights/" + QString::number(lightId) + "/state");
    selectAction.insert("method", "PUT");
    QVariantMap body;
    body.insert("alert", "select");
    selectAction.insert("body", body);
    actions.append(selectAction);

    /////////////

    QVariantMap createScheduleCommandBody;
    createScheduleCommandBody.insert("on", false);

    QVariantMap createScheduleCommand;
    createScheduleCommand.insert("address", "/api/" + HueBridgeConnection::instance()->apiKey() + "/lights/" + QString::number(lightId) + "/state");
    createScheduleCommand.insert("method", "PUT");
    createScheduleCommand.insert("body", createScheduleCommandBody);

    QTime timeFromNow(0, 1, 0);
    QString timeString = "PT" + timeFromNow.toString("hh:mm:ss");

    QVariantMap createSchedule;
    createSchedule.insert("name", "A");
    createSchedule.insert("command", createScheduleCommand);
    createSchedule.insert("localtime", timeString);


    QVariantMap timerAction;
    timerAction.insert("address", "/schedules");
    timerAction.insert("method", "PUT");
    timerAction.insert("body", createSchedule);

    actions.append(timerAction);

    return actions;
}

bool Rules::busy() const
{
    return m_busy;
}

void Rules::refresh()
{
    HueBridgeConnection::instance()->get("rules", this, "rulesReceived");
    m_busy = true;
    emit busyChanged();
}

void Rules::rulesReceived(int id, const QVariant &variant)
{
//    qDebug() << "**** rules received" << variant;
    Q_UNUSED(id)
    QVariantMap rules = variant.toMap();
    QList<Rule*> removedRules;
    foreach (Rule *rule, m_list) {
        if (!rules.contains(rule->id())) {
            removedRules.append(rule);
        }
    }

    foreach (Rule *rule, removedRules) {
        int index = m_list.indexOf(rule);
        beginRemoveRows(QModelIndex(), index, index);
        m_list.takeAt(index)->deleteLater();
        endRemoveRows();
    }

    foreach (const QString &ruleId, rules.keys()) {
        Rule *rule = findRule(ruleId);
        QVariantMap ruleMap = rules.value(ruleId).toMap();
        if (!rule) {
            rule = createRuleInternal(ruleId, ruleMap.value("name").toString());
        }
        rule->setConditions(ruleMap.value("conditions").toList());
        rule->setActions(ruleMap.value("actions").toList());
    }
    m_busy = false;
    emit busyChanged();
}

void Rules::ruleDeleted(int, const QVariant &response)
{
    qDebug() << "rule deleted" << response;
}

void Rules::createRuleFinished(int id, const QVariant &response)
{
    Q_UNUSED(id)
    qDebug() << "create rule finished:" << response;
}

//void Schedules::deleteSchedule(const QString &id)
//{
//    HueBridgeConnection::instance()->deleteResource("schedules/" + id, this, "deleteScheduleFinished");
//}

Rule* Rules::createRuleInternal(const QString &id, const QString &name)
{
    Rule *rule = new Rule(id, name, this);

//    connect(sensor, SIGNAL(nameChanged()), this, SLOT(sceneNameChanged()));
//    connect(sensor, SIGNAL(activeChanged()), this, SLOT(sceneActiveChanged()));

    beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
    m_list.append(rule);
    endInsertRows();
    return rule;
}

//void Schedules::createScheduleFinished(int id, const QVariant &response)
//{
//    Q_UNUSED(id)
//    qDebug() << "got createScene result" << response;

//    QVariantMap result = response.toList().first().toMap();

//    if (result.contains("success")) {
//        //TODO: could be added without refrshing, but we don't know the name at this point.
//        //TODO: might be best to ctor groups/lights with id only and make them fetch their own info.
//        refresh();
//    }
//}

//void Schedules::deleteScheduleFinished(int id, const QVariant &response)
//{
//    Q_UNUSED(id)
//    qDebug() << "got deleteSchedule result" << response;

//    QVariantMap result = response.toList().first().toMap();

//    if (result.contains("success")) {
//        //TODO: could be deleted without refrshing
//        refresh();
//    }
//}
