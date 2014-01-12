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

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Hue 0.1

Empty {
    id: root
    clip: true
    opacity: light && light.reachable ? 1 : .5

    property int expandedHeight: delegateColumn.height
    property var light

    property bool __isExpanded: ListView.view.expandedItem == root

    states: [
        State {
            name: "expanded"; when: root.__isExpanded
            PropertyChanges { target: root; height: root.expandedHeight + units.gu(2) }
        },
        State {
            name: "rename"
            PropertyChanges { target: mainRow; opacity: 0 }
            PropertyChanges { target: renameRow; opacity: 1 }
        }

    ]
    transitions: [
        Transition {
            from: "*"; to: "*"
            UbuntuNumberAnimation { properties: "height" }
            UbuntuNumberAnimation { properties: "opacity" }
        }
    ]



    onClicked: {
        if (light.reachable) {
            if (ListView.view.expandedItem == root) {
                ListView.view.expandedItem = null;
            } else {
                ListView.view.expandedItem = root;
            }
        }
    }
    onPressAndHold: {
        if (delegateItem.state == "rename") {
            delegateItem.state = ""
        } else {
            delegateItem.state = "rename"
        }
    }

    Column {
        id: delegateColumn
        anchors { left: parent.left; right: parent.right; leftMargin: units.gu(2); rightMargin: units.gu(2) }
        spacing: units.gu(2)
        height: childrenRect.height

        Connections {
            target: root.light
            onStateChanged: {
                brightnessSlider.value = root.light.bri;
                onOffSwitch.checked = root.light.on;
                effectSelector.selectedIndex = effectSelector.findIndex();
            }
            onNameChanged: {
                nameLabel.text = root.light.name;
            }
        }

        Item {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: units.gu(6)
            Row {
                id: mainRow
                anchors.fill: parent
                spacing: units.gu(1)
                visible: opacity > 0

                Icon {
                    id: icon
                    height: parent.height - units.gu(2)
                    anchors.verticalCenter: parent.verticalCenter
                    width: height
                    name: light && light.reachable ? light.on ? "torch-on" : "torch-off" : "flash-off"
                }

                Label {
                    id: nameLabel
                    width: parent.width - onOffSwitch.width - icon.width - parent.spacing*2
                    text: light ? light.name : ""
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id: onOffSwitch
                    checked: light && light.on
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        light.on = checked;
                    }
                }
            }
            Row {
                id: renameRow
                anchors.fill: parent
                spacing: units.gu(2)
                height: units.gu(6)
                visible: opacity > 0
                opacity: 0

                TextField {
                    id: renameTextField
                    width: parent.width - okButton.width - parent.spacing
                    anchors.verticalCenter: parent.verticalCenter
                    text: light ? light.name : ""
                }
                Button {
                    id: okButton
                    text: "OK"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        light.name = renameTextField.text
                        delegateItem.state = ""
                    }
                }
            }
        }

        Row {
            anchors { left: parent.left; right: parent.right }
            spacing: units.gu(1)
            Icon {
                height: brightnessSlider.height
                width: height
                name: "torch-off"
            }
            Slider {
                id: brightnessSlider
                width: parent.width - height * 2 - parent.spacing * 2
                minimumValue: 0
                maximumValue: 255
                value: light ? light.bri : 0
                onValueChanged: {
                    light.bri = value
                }
            }
            Icon {
                height: brightnessSlider.height
                width: height
                name: "torch-on"
            }
        }


        UbuntuColorPicker {
            id: colorPicker
            anchors { left: parent.left; right: parent.right }
            height: width / 3
            color: light ? light.color : "black"
            active: light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false

            touchDelegate: UbuntuShape {
                height: units.gu(3)
                width: units.gu(3)
                color: "black"
            }

            onColorChanged: {
                if (pressed) {
                    light.color = colorPicker.color;
                }
            }
        }

        UbuntuColorPickerCt {
            id: colorPickerCt
            anchors { left: parent.left; right: parent.right }
            height: width / 6
            ct: light ? light.ct : minCt
            active: light && light.colormode == LightInterface.ColorModeCT

            onCtChanged: {
                if (pressed) {
                    light.ct = colorPickerCt.ct;
                }
            }

            touchDelegate: Rectangle {
                height: colorPickerCt.height
                width: units.gu(.5)
                color: "transparent"
                border.color: "black"
                border.width: units.dp(2)
            }
        }

        OptionSelector {
            id: effectSelector
            model: ListModel {
                id: effectModel
                ListElement { name: "No effect"; value: "none" }
                ListElement { name: "Color loop"; value: "colorloop" }
            }
            selectedIndex: findIndex()

            function findIndex() {
                if (!light) {
                    return 0;
                }

                for (var i = 0; i < effectModel.count; i++) {
                    if (effectModel.get(i).value == light.effect) {
                        return i;
                    }
                }
                return 0;
            }

            onSelectedIndexChanged: {
                light.effect = effectModel.get(selectedIndex).value;
            }

            delegate: OptionSelectorDelegate {
                text: name
            }
        }
    }
}
