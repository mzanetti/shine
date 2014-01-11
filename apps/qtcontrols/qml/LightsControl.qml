/*
 * Copyright 2013 Michael Zanetti
 *                Christian Muehlhaeuser
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
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

GridLayout {
    id: layout
    columns: 2
    property var light

    onLightChanged: {
        effectCb.update();
    }

    Label {
        text: "Power: " + (light && light.on ? "On" : "Off")
    }
    Button {
        text: "Switch " + (light && light.on ? "Off" : "On")
        Layout.fillWidth: true
        onClicked: {
            light.on = !light.on
        }
    }
    Label {
        text: "Brightness:"
    }
    Slider {
        Layout.fillWidth: true
        minimumValue: 0
        maximumValue: 255
        value: light ? light.bri : 0
        onValueChanged: {
            light.bri = value
        }
    }

    ColorPicker {
        enabled: effectCb.currentIndex != 1
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.columnSpan: 2
        color: light.color
        active: light.colormode == LightInterface.ColorModeHS || light.color == LightInterface.ColorModeXY
        onColorChanged: {
            if (pressed) {
                light.color = color;
            }
        }
        touchDelegate: Rectangle {
            height: effectLabel.height / 2
            width: height
            radius: height * .499
            color: "black"
        }
    }

    ColorPickerCt {
        id: colorPickerCt
        enabled: effectCb.currentIndex != 1
        Layout.fillWidth: true
        height: effectLabel.height * 2
        Layout.columnSpan: 2
        ct: light.ct
        active: light.colormode == LightInterface.ColorModeCT

        onCtChanged: {
            if (pressed) {
                light.ct = ct;
            }
        }
        touchDelegate: Rectangle {
            height: colorPickerCt.height
            width: effectLabel.height / 4
            color: "transparent"
            border.width: width / 3
            border.color: "black"
        }
    }

    Label {
        id: effectLabel
        text: "Effect:"
    }

    ComboBox {
        id: effectCb
        Layout.fillWidth: true
        textRole: "name"
        model: ListModel {
            id: effectModel
            ListElement { name: "No effect"; value: "none" }
            ListElement { name: "Color loop"; value: "colorloop" }
        }
        function update(){
            if (light === null || light === undefined) {
                effectCb.currentIndex = -1;
                return;
            }

            for (var i = 0; i < effectModel.count; i++) {
                if (effectModel.get(i).value == light.effect) {
                    effectCb.currentIndex = i;
                }
            }
            return effectCb.currentIndex = -1;
        }

        onCurrentIndexChanged: {
            if (light && currentIndex > -1) {
                light.effect = effectModel.get(currentIndex).value;
            }
        }
    }
}

