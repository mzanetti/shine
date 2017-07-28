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
import QtQuick.Controls 2.1
import Hue 0.1

ItemDelegate {
    id: root
    property var light
    property var schedules: null

    signal changed()

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

//    leadingActions: root.light.isGroup && root.light.id !== 0 ? deleteAction : null
//    ListItemActions {
//        id: deleteAction
//        actions: [
//            Action {
//                iconName: "delete"
//                onTriggered: groups.deleteGroup(groups.get(index).id)
//            }
//        ]
//    }
//    trailingActions: ListItemActions {
//        actions: [
//            Action {
//                iconName: "alarm-clock"
//                onTriggered: {
//                    PopupUtils.open(Qt.resolvedUrl("CreateAlarmDialog.qml"), root, {light: root.light, schedules: root.schedules})
//                }
//            },
//            Action {
//                iconName: "camera-self-timer"
//                onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateTimerDialog.qml"), root, {light: root.light, schedules: root.schedules })
//            },
//            Action {
//                iconName: "edit"
//                onTriggered: root.onPressAndHold()
//            }
//        ]
//    }

    Row {
        id: mainRow
        anchors {
            fill: parent
            leftMargin: 8 * (2);
            rightMargin: 8 * (2)
        }
        spacing: 8 * (1)

        Image {
            id: icon
            height: parent.height - 8 * (2)
            anchors.verticalCenter: parent.verticalCenter
            width: height
            source: {
                if (!light) return "";
                var name = "images/";
                print("modelId", light.modelId)
                switch(light.modelId) {
                case "LCT001":
                case "LWB004":
                case "LWB006":
                    name += "a19";
                    break;
                case "LCT002":
                    name += "br30";
                    break;
                case "LCT003":
                    name += "gu10";
                    break;
                case "LST001":
                    name += "lightstrip";
                    break;
                case "LLC010":
                case "LLC006":
                    name += "lc_iris";
                    break;
                case "LLC011":
                case "LLC012":
                case "LLC007":
                    name += "lc_bloom";
                    break;
                case "LLC013":
                    name += "storylight";
                    break;
                case "LLC020":
                    name += "huego";
                    break;
                case "HBL001":
                case "HBL002":
                case "HBL003":
                    name += "beyond";
                    break;
                case "HEL001":
                case "HEL002":
                    name += "entity";
                    break;
                case "HIL001":
                case "HIL002":
                    name += "impulse";
                    break;
                case "HML001":
                case "HML002":
                case "HML003":
                case "HML007":
                    name += "phoenix";
                    break;
                default:
                    name += "group"
                }
                name += "_" + (light.reachable ? "outline" : "filled") + ".svg"
                return name;
            }

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
                root.changed();
            }
        }
    }

    Component {
        id: renameDialog
        Dialog {
            id: rd
            title: "Rename %1".arg(root.light.isGroup ? "Group" : "Light")
//            text: "Please enter the new name for the %1".arg(root.light.isGroup ? "Group" : "Light")
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
