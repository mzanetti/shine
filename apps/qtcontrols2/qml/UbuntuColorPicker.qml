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
import Hue 0.1

Item {

    property alias color: colorPicker.color
    property alias pressed: colorPicker.pressed
    property alias touchDelegate: colorPicker.touchDelegate
    property alias lights: colorPicker.lights
    property alias active: colorPicker.active

    function calculateColor(x, y) {
        return colorPicker.calculateColor(x, y);
    }

    function calculateXy(color) {
        return colorPicker.calculateXy(color);
    }

    ShaderEffectSource {
        id: source
        anchors.centerIn: parent
        width: 1
        height: 1
        hideSource: true
        sourceItem: colorPicker
    }

    Rectangle {
        id: shape
        source: source

        anchors.fill: parent
    }

    ColorPicker {
        id: colorPicker
        anchors.fill: parent
    }

}
