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

    property alias ct: colorPickerCt.ct
    property alias minCt: colorPickerCt.minCt
    property alias maxCt: colorPickerCt.maxCt
    property alias touchDelegate: colorPickerCt.touchDelegate
    property alias pressed: colorPickerCt.pressed
    property alias active: colorPickerCt.active

    ShaderEffectSource {
        id: source
        anchors.centerIn: parent
        width: 1
        height: 1
        hideSource: true
        sourceItem: colorPickerCt
    }

    Shape {
        id: shape
        source: source

        anchors.fill: parent
    }

    ColorPickerCt {
        id: colorPickerCt
        anchors.fill: parent
    }

}
