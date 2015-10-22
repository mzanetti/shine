/*
 * Copyright 2015 Steffen Köhler
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
 *      Steffen Köhler <mail@steffenkoehler.net>
 */

#include "user.h"

User::User(const QString &id, const QString &name, const QString &lastUsed, const QString &created):
    m_id(id),
    m_name(name),
    m_lastUsed(lastUsed),
    m_created(created)
{
}

QString User::name() const
{
    return m_name;
}

QString User::id() const
{
    return m_id;
}

QString User::lastUsed() const
{
    return m_lastUsed;
}

QString User::created() const
{
    return m_created;
}
