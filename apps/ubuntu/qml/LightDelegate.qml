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

ListItem {
    id: root
    property var light
    property var schedules: null

    onClicked: {
        pageStack.push(Qt.resolvedUrl("LightDetailsPage.qml"), {light: root.light, schedules: root.schedules})
    }
    onPressAndHold: {
        PopupUtils.open(renameDialog, this)
        root.light.alert = "lselect"
    }

    Connections {
        target: root.light
        onStateChanged: {
            onOffSwitch.checked = root.light.on;
        }
        onNameChanged: {
            nameLabel.text = root.light.name;
        }
    }

    leadingActions: root.light.isGroup && root.light.id !== 0 ? deleteAction : null
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
                onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateTimerDialog.qml"), root, {light: root.light, schedules: root.schedules })
            },
            Action {
                iconName: "edit"
                onTriggered: root.onPressAndHold()
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

        Image {
            id: icon
            height: parent.height - units.gu(2)
            anchors.verticalCenter: parent.verticalCenter
            width: height
            source: light && light.reachable ? "images/a19_outline.svg" : "images/a19_filled.svg"
            sourceSize.width: width
            sourceSize.height: height
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

    Component {
        id: renameDialog
        Dialog {
            id: rd
            title: "Rename %1".arg(root.light.isGroup ? "Group" : "Light")
            text: "Please enter the new name for the %1".arg(root.light.isGroup ? "Group" : "Light")
            TextField {
                id: renameTextField
                width: parent.width - okButton.width - parent.spacing
                text: light ? light.name : ""
            }
            Button {
                id: okButton
                text: "OK"
                onClicked: {
                    root.light.alert = "none"
                    root.light.name = renameTextField.text
                    PopupUtils.close(rd)
                }
            }
        }
    }
}
