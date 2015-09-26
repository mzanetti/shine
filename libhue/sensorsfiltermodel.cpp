#include "sensorsfiltermodel.h"

#include "sensor.h"
#include "sensors.h"

#include <QDebug>

SensorsFilterModel::SensorsFilterModel(QObject *parent):
    QSortFilterProxyModel(parent),
    m_sensors(0),
    m_shownTypes(Sensor::TypeAll)
{
}

Sensors *SensorsFilterModel::sensors() const
{
    return m_sensors;
}

void SensorsFilterModel::setSensors(Sensors *sensors)
{
    if (m_sensors != sensors) {
        if (m_sensors) {
            disconnect(m_sensors, SIGNAL(countChanged()), this, SIGNAL(countChanged()));
        }
        m_sensors = sensors;
        setSourceModel(sensors);
        connect(m_sensors, SIGNAL(countChanged()), this, SIGNAL(countChanged()));
        emit sensorsChanged();
        emit countChanged();
    }
}

Sensor::Types SensorsFilterModel::shownTypes() const
{
    return m_shownTypes;
}

void SensorsFilterModel::setShownTypes(Sensor::Types types)
{
    if (m_shownTypes != types) {
        m_shownTypes = types;
        emit shownTypesChanged();
        invalidateFilter();
        emit countChanged();
    }
}

Sensor *SensorsFilterModel::get(int row) const
{
    QModelIndex proxyIndex = index(row, 0);
    return m_sensors->get(mapToSource(proxyIndex).row());
}

bool SensorsFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (m_shownTypes != Sensor::TypeAll && !m_shownTypes.testFlag(m_sensors->get(sourceRow)->type())) {
        return false;
    }
    return true;
}
