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
import Hue 0.1

Page {
    id: root
    title: "Lights"

    property alias lights: lightsFilterModel.lights
    property var groups: null
    property var schedules: null

    head {
        actions: [
            Action {
                text: "group"
                iconName: "add"
                onTriggered: {
                    var popup = PopupUtils.open(Qt.resolvedUrl("CreateDialog.qml"), root, {mode: "group", lights: root.lights})
                    popup.accepted.connect(function(name, lightsList) {
                        groups.createGroup(name, lightsList);
                    })
                }
            }
        ]
    }

    Item { // wrap flickable to disable header fancyness
        anchors.fill: parent
        clip: true

        Flickable {
            id: mainFlickable
            anchors.fill: parent
            contentHeight: mainColumn.height
            interactive: contentHeight > height

            Column {
                id: mainColumn
                anchors { left: parent.left; right: parent.right }

                add: Transition {
                    UbuntuNumberAnimation { properties: "opacity"; from: 0 }
                }
                move: Transition {
                    UbuntuNumberAnimation { properties: "x,y" }
                }

                Label {
                    text: "Groups"
                    anchors {left: parent.left; right: parent.right; margins: units.gu(2) }
                    height: units.gu(3)
                    verticalAlignment: Text.AlignVCenter
                }

                ThinDivider {}

                Repeater {
                    model: groups

                    delegate: LightDelegate {
                        light: groups.get(index)
                        schedules: root.schedules
                        Connections {
                            target: groups.get(index)
                            onWriteOperationFinished: {
                                root.lights.refresh()
                            }
                        }
                    }
                }

                Label {
                    anchors {left: parent.left; right: parent.right; margins: units.gu(2) }
                    text: "Lights"
                    height: units.gu(3)
                    verticalAlignment: Text.AlignVCenter
                }

                ThinDivider {}

                Repeater {
                    model: LightsFilterModel {
                        id: lightsFilterModel
                    }
                    delegate: LightDelegate {
                        light: lightsFilterModel.get(index)
                        schedules: root.schedules
                        Connections {
                            target: lightsFilterModel.get(index)
                            onWriteOperationFinished: {
                                root.groups.refresh()
                            }
                        }
                    }
                }
            }
        }
    }
}

