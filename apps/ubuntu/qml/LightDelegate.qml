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
import Ubuntu.Components.ListItems 1.3
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
            PropertyChanges { target: root; implicitHeight: root.expandedHeight + units.gu(2) }
        },
        State {
            name: "rename"
            PropertyChanges { target: mainRow; opacity: 0 }
            PropertyChanges { target: renameRow; opacity: 1 }
            PropertyChanges { target: root.light; alert: "lselect" }
        }

    ]
    transitions: [
        Transition {
            from: "*"; to: "*"
            UbuntuNumberAnimation { properties: "implicitHeight" }
            UbuntuNumberAnimation { properties: "opacity" }
        }
    ]



    onClicked: {
//        if (light.reachable) {
            if (ListView.view.expandedItem == root) {
                ListView.view.expandedItem = null;
            } else {
                ListView.view.expandedItem = root;
            }
//        }
    }
    onPressAndHold: {
        if (root.state == "rename") {
            root.state = ""
        } else {
            root.state = "rename"
        }
    }

    MouseArea {
        anchors.fill: delegateColumn
        anchors.topMargin: units.gu(6)
        enabled: root.__isExpanded
        preventStealing: true
    }

    Column {
        id: delegateColumn
        anchors { left: parent.left; right: parent.right; }
        spacing: units.gu(2)
        height: childrenRect.height

        Connections {
            target: root.light
            onStateChanged: {
                brightnessSlider.value = root.light.bri;
                onOffSwitch.checked = root.light.on;
                effectSelector.selectedIndex = effectSelector.findIndex();
                if (!colorPicker.pressed || !colorPicker.active) {
                    colorPicker.color = root.light.color;
                }
                colorPicker.active = light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false
                if (!colorPickerCt.pressed) {
                    colorPickerCt.ct = root.light.ct;
                }
                colorPickerCt.active = !colorPicker.active
            }
            onNameChanged: {
                nameLabel.text = root.light.name;
            }
        }

        ListItem {
            anchors {
                left: parent.left
                right: parent.right
            }
            height: units.gu(6) + units.dp(2)
            divider { colorFrom: "transparent"; colorTo: "transparent" }
            onClicked: root.clicked()
            onPressAndHold: root.pressAndHold()
            leadingActions: groups.get(index).id == 0 ? null : deleteAction
            ListItemActions {
                id: deleteAction
                actions: [
                    Action {
                        iconName: "delete"
                        onTriggered: groups.deleteGroup(groups.get(index).id)
                    }
                ]
            }
            trailingActions: ListItemActions {
                actions: [
                    Action {
                        iconName: "alarm-clock"
                    },
                    Action {
                        iconName: "camera-self-timer"
                    },
                    Action {
                        iconName: "edit"
                        onTriggered: root.state = "rename"
                    }

                ]
            }

            Row {
                id: mainRow
                anchors {
                    fill: parent
                    leftMargin: units.gu(2);
                    rightMargin: units.gu(2)
                }
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
                anchors {
                    fill: parent
                    leftMargin: units.gu(2);
                    rightMargin: units.gu(2)
                }
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
                        root.state = ""
                    }
                }
            }
        }

        Row {
            anchors {
                left: parent.left;
                right: parent.right
                leftMargin: units.gu(2);
                rightMargin: units.gu(2)
            }
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
            anchors {
                left: parent.left;
                right: parent.right
                leftMargin: units.gu(2);
                rightMargin: units.gu(2)
            }
            height: width / 3
            color: light ? light.color : "black"
            active: light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false

            touchDelegate: UbuntuShape {
                height: units.gu(3)
                width: units.gu(3)
                backgroundColor: "black"
            }

            onColorChanged: {
                if (pressed) {
                    light.color = colorPicker.color;
                }
            }
        }

        UbuntuColorPickerCt {
            id: colorPickerCt
            anchors {
                left: parent.left;
                right: parent.right
                leftMargin: units.gu(2);
                rightMargin: units.gu(2)
            }
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

        ItemSelector {
            id: effectSelector
            anchors {
                left: parent.left;
                right: parent.right
                leftMargin: units.gu(2);
                rightMargin: units.gu(2)
            }
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
