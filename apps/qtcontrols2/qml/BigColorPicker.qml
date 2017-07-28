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
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Hue 0.1

Rectangle {
    id: root
    property var lights: null
    color: "white"

    LightsFilterModel {
        id: filterModel
        lights: root.lights
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: units.gu(1)
        spacing: units.gu(1)
        ColorPicker {
            id: bigColorPicker
            Layout.fillHeight: true
            Layout.fillWidth: true
            lights: filterModel

            touchDelegate: Rectangle {
                id: lightDelegate
                color: light && light.reachable ? (light.on ? "blue" : "gray") : "red"
                height: units.gu(5)
                width: units.gu(5)
                property var point: light ? bigColorPicker.calculateXy(light.color) : undefined
//                radius: "medium"
                opacity: light && light.reachable ? 1 : .5
                Label {
                    id: label
                    color: "white"
                    anchors.centerIn: parent
                    text: light && light.id
                }

                property var light
            }
        }

        ListView {
            Layout.fillHeight: true
            Layout.preferredWidth: units.gu(20)
            model: root.lights
            spacing: units.gu(1)
            delegate: RowLayout {
                CheckBox {
                    checked: true
                    onCheckedChanged: {
                        if (!checked) {
                            filterModel.hideLight(model.id)
                        } else {
                            filterModel.showLight(model.id)
                        }
                    }
                }
                Label {
                    text: model.id + " " + model.name
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}

