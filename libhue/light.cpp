#include "light.h"


Light::Light(int id, QObject *parent):
    QObject(parent),
    m_id(id)
{

}

int Light::id() const
{
    return m_id;
}

QString Light::name() const
{
    return m_name;
}

void Light::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}
