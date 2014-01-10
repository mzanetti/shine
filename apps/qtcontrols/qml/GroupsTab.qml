/*
 * Copyright 2013 Michael Zanetti
 *                Christian Muehlhaeuser
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
 *      Christian Muehlhaeuser <muesli@gmail.com>
 */

import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

Tab {
    id: groupTab

    SplitView {
        ColumnLayout {
            id: layout2
            TableView {
                id: groupsView
                Layout.fillWidth: true
                Layout.fillHeight: true

                model: Groups {
                    id: groups
                }

                TableViewColumn {
                    role: "name"
                    title: "Hue groups"
                }
            }
            Button {
                id: deleteButton
                Layout.fillWidth: true
                property var group: groups.get(groupsView.currentRow)
                text: "Delete Group"
                onClicked: {
                    groups.deleteGroup(deleteButton.group.id);
                }
            }
            Button {
                id: addButton
                Layout.fillWidth: true
                text: "Add"
                onClicked: {
                    var component = Qt.createComponent("NewGroupWindow.qml");
                    var window = component.createObject(groupTab);
                    window.show();
                    window.accepted.connect(function(name, lights) {
                        groups.createGroup(name)
                    });
                }
            }
        }

        Item {
            Layout.fillWidth: true
            LightsControl {
                anchors.fill: parent
                anchors.margins: 20
                visible: groupsView.currentRow > -1
                light: groups.get(groupsView.currentRow)
            }
        }
    }
}
