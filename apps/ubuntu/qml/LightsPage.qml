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

Page {
    title: "Lights"
    ListView {
        id: lightsListView
        anchors.fill: parent

        property var expandedItem: null

        model: Lights {
            id: lights
        }

        delegate: LightDelegate {
            id: delegateItem
            light: lights.get(index)

            states: [
                State {
                    name: "expanded"; when: lightsListView.expandedItem === delegateItem
                    PropertyChanges { target: delegateItem; height: delegateItem.expandedHeight + units.gu(2) }
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
                    if (ListView.view.expandedItem == delegateItem) {
                        ListView.view.expandedItem = null;
                    } else {
                        ListView.view.expandedItem = delegateItem;
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
        }
    }
}

