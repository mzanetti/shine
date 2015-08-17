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

import QtQuick 2.3
import Ubuntu.Components 1.3
import Hue 0.1

Item {
    property alias lights: bigColorPicker.lights

    UbuntuColorPicker {
        id: bigColorPicker
        anchors.fill: parent
        anchors.margins: units.gu(2)

        touchDelegate: UbuntuShape {
            id: lightDelegate
            backgroundColor: light && light.reachable ? (light.on ? "blue" : "gray") : "red"
            height: units.gu(5)
            width: units.gu(5)
            property var point: light ? bigColorPicker.calculateXy(light.color) : undefined
            radius: "medium"
            opacity: light && light.reachable ? 1 : .5
            Label {
                id: label
                color: "white"
                anchors.centerIn: parent
            }

            property var light
            property alias text: label.text
        }
    }
}
