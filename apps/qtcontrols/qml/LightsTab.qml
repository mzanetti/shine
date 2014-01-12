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
    id: root
    property var lights

    SplitView {
        anchors.fill: parent


        ColumnLayout {
            ComboBox {
                id: groupsComboBox
                Layout.fillWidth: true
                model: Groups {
                    id: groups
                }
                textRole: "name"
                onCurrentIndexChanged: {
                    lightsFilterModel.groupId = groups.get(currentIndex).id
                }
            }

            TableView {

                id: lightsView
                model: LightsFilterModel {
                    id: lightsFilterModel
                    lights: root.lights
                }
                Layout.fillHeight: true

                TableViewColumn {
                    role: "name"
                    title: "Hue bulbs"
                }
            }

            Button {
                id: deleteButton
                Layout.fillWidth: true
                property var group: groups.get(groupsView.currentRow)
                text: "Delete Group"
                enabled: groupsComboBox.currentIndex > 0
                onClicked: {
                    groups.deleteGroup(groups.get(groupsComboBox.currentIndex).id);
                    groupsComboBox.currentIndex = 0;
                }
            }
            Button {
                id: addButton
                Layout.fillWidth: true
                text: "Add"
                onClicked: {
                    var component = Qt.createComponent("NewGroupWindow.qml");
                    var window = component.createObject(root, {lights: root.lights});
                    window.show();
                    window.accepted.connect(function(name, lights) {
                        groups.createGroup(name, lights)
                    });
                }
            }
        }

        Item {
            LightsControl {
                anchors.fill: parent
                anchors.margins: 20
                visible: lightsView.currentRow > -1
                light: root.lights.get(lightsView.currentRow)
            }
        }
    }
}
