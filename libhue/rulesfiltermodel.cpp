#include "rulesfiltermodel.h"

#include "rule.h"
#include "rules.h"

#include <QDebug>

RulesFilterModel::RulesFilterModel(QObject *parent):
    QSortFilterProxyModel(parent),
    m_rules(0)
{


}

Rules *RulesFilterModel::rules() const
{
    return m_rules;
}

void RulesFilterModel::setRules(Rules *rules)
{
    if (m_rules != rules) {
        if (m_rules) {
            disconnect(m_rules, SIGNAL(countChanged()), this, SIGNAL(countChanged()));
        }
        m_rules = rules;
        setSourceModel(rules);
        emit rulesChanged();
        connect(m_rules, SIGNAL(countChanged()), this, SIGNAL(countChanged()));
        emit countChanged();
    }
}

QVariantMap RulesFilterModel::conditionFilter() const
{
    return m_conditionFilter;
}

void RulesFilterModel::setConditionFilter(const QVariantMap &conditionFilter)
{
    if (m_conditionFilter != conditionFilter) {
        m_conditionFilter = conditionFilter;
        emit conditionFilterChanged();
        invalidateFilter();
        emit countChanged();
    }
}

Rule *RulesFilterModel::get(int row) const
{
    QModelIndex proxyIndex = index(row, 0);
    return m_rules->get(mapToSource(proxyIndex).row());
}

bool RulesFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (!m_conditionFilter.isEmpty()) {
        bool match = false;
        foreach (const QVariant &condition, m_rules->get(sourceRow)->conditions()) {
            if (condition.toMap() == m_conditionFilter) {
                match = true;
            }
        }
        if (!match) {
            return false;
        }
    }
    return true;
}
