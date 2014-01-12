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
    id: root
    title: "Lights"

    property alias lights: lightsFilterModel.lights

    Column {
        anchors.fill: parent

        OptionSelector {
            model: Groups {
                id: groups
            }

            delegate: OptionSelectorDelegate {
                text: name
            }

            onSelectedIndexChanged: {
                lightsFilterModel.groupId = groups.get(selectedIndex).id
            }
        }

        ListView {
            id: lightsListView
            anchors {left: parent.left; right: parent.right }
            height: parent.height - y

            model: LightsFilterModel {
                id: lightsFilterModel
            }

            property var expandedItem: null

            delegate: LightDelegate {
                id: delegateItem
                light: lightsFilterModel.get(index)
            }
        }
    }
}

