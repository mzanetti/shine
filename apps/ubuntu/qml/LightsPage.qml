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

    Groups {
        id: groups
    }

    Item { // wrap flickable to disable header fancyness
        anchors.fill: parent
        clip: true

        Flickable {
            id: mainFlickable
            anchors.fill: parent
            contentHeight: mainColumn.height
            interactive: contentHeight > height && groupsListView.expandedItem == null && lightsListView.expandedItem == null

            Column {
                id: mainColumn
                anchors { left: parent.left; right: parent.right }

                Label {
                    text: "Groups"
                    anchors {left: parent.left; right: parent.right; margins: units.gu(2) }
                    height: units.gu(3)
                    verticalAlignment: Text.AlignBottom
                }

                ThinDivider {}

                ListView {
                    id: groupsListView
                    anchors {left: parent.left; right: parent.right }
                    height: contentHeight
                    model: groups
                    interactive: false
                    property var expandedItem: null
                    onExpandedItemChanged: {
                        if (expandedItem) {
                            lightsListView.expandedItem = null
                        }
                    }

                    delegate: LightDelegate {
                        id: delegateItem
                        light: groups.get(index)

                        onHeightChanged: {
                            var y = delegateItem.mapToItem(mainFlickable).y;
                            if (y + delegateItem.height > mainFlickable.height) {
                                mainFlickable.contentY += y + delegateItem.height - mainFlickable.height;
                            }
                        }
                    }
                }

                Label {
                    anchors {left: parent.left; right: parent.right; margins: units.gu(2) }
                    text: "Lights"
                    height: units.gu(3)
                    verticalAlignment: Text.AlignBottom
                }

                ThinDivider {}

                ListView {
                    id: lightsListView
                    anchors {left: parent.left; right: parent.right }
                    height: contentHeight
                    interactive: false

                    model: LightsFilterModel {
                        id: lightsFilterModel
                    }

                    property Item expandedItem: null
                    onExpandedItemChanged: {
                        if (expandedItem) {
                            groupsListView.expandedItem = null
                        }
                    }

                    delegate: LightDelegate {
                        id: delegateItem
                        light: lightsFilterModel.get(index)

                        onHeightChanged: {
                            var y = delegateItem.mapToItem(mainFlickable).y;
                            print("at:", y)
                            if (y + delegateItem.height > mainFlickable.height) {
                                mainFlickable.contentY += y + delegateItem.height - mainFlickable.height
                            }
                            if (y < 0) {
                                mainFlickable.contentY += y
                                mainFlickable.returnToBounds()
                            }
                        }
                    }

                    add: Transition {
                        UbuntuNumberAnimation { properties: "opacity"; from: 0; to: 1 }
                    }
                    displaced: Transition {
                        UbuntuNumberAnimation { properties: "x,y" }
                    }
                }
            }
        }
    }


}

