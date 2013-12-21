/*
 * Copyright 2013 Michael Zanetti
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

#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>
#include <QPointF>
#include <QColor>

class Light: public QObject
{
    Q_OBJECT
    Q_ENUMS(ColorMode)

    Q_PROPERTY(int id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(QString modelId READ modelId NOTIFY modelIdChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString swversion READ swversion NOTIFY swversionChanged)

    Q_PROPERTY(bool on READ on WRITE setOn NOTIFY stateChanged)
    Q_PROPERTY(quint8 bri READ bri WRITE setBri NOTIFY stateChanged)
    Q_PROPERTY(quint16 hue READ hue NOTIFY stateChanged)
    Q_PROPERTY(quint8 sat READ sat NOTIFY stateChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY stateChanged)
    Q_PROPERTY(QPointF xy READ xy NOTIFY stateChanged)
    Q_PROPERTY(quint16 ct READ ct NOTIFY stateChanged)
    Q_PROPERTY(QString alert READ alert NOTIFY stateChanged)
    Q_PROPERTY(QString effect READ effect WRITE setEffect NOTIFY stateChanged)
    Q_PROPERTY(QString colormode READ colormode NOTIFY stateChanged)
    Q_PROPERTY(bool reachable READ reachable NOTIFY stateChanged)

public:
    Light(int id, const QString &name, QObject *parent = 0);

    int id() const;

    QString name() const;
    void setName(const QString &name);

    QString modelId() const;
    void setModelId(const QString &modelId);

    QString type() const;
    void setType(const QString &type);

    QString swversion() const;
    void setSwversion(const QString &swversion);


    bool on() const;
    void setOn(bool on);

    quint8 bri() const;
    void setBri(quint8 bri);

    quint16 hue() const;
    void setHue(quint16 hue);

    quint8 sat() const;
    void setSat(quint8 sat);

    QColor color() const;
    void setColor(const QColor &color);

    QPointF xy() const;
    void setXy(const QPointF &xy);

    quint16 ct() const;
    void setCt(quint16 ct);

    QString alert() const;
    void setAlert(const QString &alert);

    QString effect() const;
    void setEffect(const QString &effect);

    QString colormode() const;
    void setColormode(const QString &colormode);

    bool reachable() const;
    void setReachable(bool reachable);

signals:
    void nameChanged();

    void modelIdChanged();
    void typeChanged();
    void swversionChanged();

    void stateChanged();

private slots:
    void responseReceived(int id, const QVariant &response);
    void setDescriptionFinished(int id, const QVariant &response);
    void setStateFinished(int id, const QVariant &response);

private:
    int m_id;
    QString m_name;

    QString m_modelId;
    QString m_type;
    QString m_swversion;

    bool m_on;
    quint8 m_bri;
    quint16 m_hue;
    quint8 m_sat;
    QPointF m_xy;
    quint16 m_ct;
    QString m_alert;
    QString m_effect;
    QString m_colormode;
    bool m_reachable;

    int m_setColorId;
    bool m_outOfSync;
};

#endif
