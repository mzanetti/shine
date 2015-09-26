#ifndef SENSORSFILTERMODEL_H
#define SENSORSFILTERMODEL_H

#include "sensor.h"

#include <QSortFilterProxyModel>

class Sensors;

class SensorsFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(Sensors* sensors READ sensors WRITE setSensors NOTIFY sensorsChanged)
    Q_PROPERTY(Sensor::Types shownTypes READ shownTypes WRITE setShownTypes NOTIFY shownTypesChanged)

public:
    SensorsFilterModel(QObject *parent = 0);

    Sensors* sensors() const;
    void setSensors(Sensors *sensors);

    Sensor::Types shownTypes() const;
    void setShownTypes(Sensor::Types types);

    Q_INVOKABLE Sensor* get(int row) const;

    int count() { return rowCount(); }
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

signals:
    void countChanged();
    void sensorsChanged();
    void shownTypesChanged();

private:
    Sensors *m_sensors;
    Sensor::Types m_shownTypes;
};

#endif
