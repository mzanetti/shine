#ifndef RULESFILTERMODEL_H
#define RULESFILTERMODEL_H

#include <QSortFilterProxyModel>

class Rule;
class Rules;

class RulesFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Rules* rules READ rules WRITE setRules NOTIFY rulesChanged)
    Q_PROPERTY(QVariantMap conditionFilter READ conditionFilter WRITE setConditionFilter NOTIFY conditionFilterChanged)

public:
    RulesFilterModel(QObject *parent = 0);

    Rules* rules() const;
    void setRules(Rules *rules);

    bool hideOtherApps() const;
    void setHideOtherApps(bool hideOtherApps);

    QVariantMap conditionFilter() const;
    void setConditionFilter(const QVariantMap &conditionFilter);

    int count() { return rowCount(); }
    Q_INVOKABLE Rule* get(int row) const;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void countChanged();
    void rulesChanged();
    void conditionFilterChanged();

private:
    Rules *m_rules;
    QVariantMap m_conditionFilter;
};

#endif
