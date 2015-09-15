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
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.Pickers 1.3
import Hue 0.1

Empty {
    id: root
    clip: true
    opacity: light && light.reachable ? 1 : .5

    property var light
    property var schedules: null

    states: [
        State {
            name: "rename"
            PropertyChanges { target: mainRow; opacity: 0 }
            PropertyChanges { target: renameRow; opacity: 1 }
            PropertyChanges { target: root.light; alert: "lselect" }
        }

    ]

    onClicked: {
        pageStack.push(Qt.resolvedUrl("LightDetailsPage.qml"), {light: root.light, schedules: root.schedules})
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
                onOffSwitch.checked = root.light.on;
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
                        onTriggered: {
                            PopupUtils.open(Qt.resolvedUrl("CreateAlarmDialog.qml"), root, {light: root.light, schedules: root.schedules})
                        }
                    },
                    Action {
                        iconName: "camera-self-timer"
                        onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateTimerDialog.qml"), root)
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
                        light.on = !light.on;
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
    }
}
