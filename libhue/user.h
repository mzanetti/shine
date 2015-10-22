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

#ifndef USER_H
#define USER_H

#include <QString>

class User
{

public:
    User(const QString &id, const QString &name, const QString &lastUsed, const QString &created);

    QString name() const;
    QString id() const;
    QString lastUsed() const;
    QString created() const;

private:
    QString m_id;
    QString m_name;

    QString m_lastUsed;
    QString m_created;
};

#endif
