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
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

ApplicationWindow {
    id: root
    width: mainColumn.implicitWidth * 1.5
    minimumWidth: mainColumn.implicitWidth
    height: mainColumn.implicitHeight
    minimumHeight: mainColumn.implicitHeight
    title: "Add Group"

    property int spacing: rowLayout.height / 4
    property alias lights: lightsRepeater.model

    signal accepted(string name, var lights)
    signal rejected()

    Component.onDestruction: print("destroying window")

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent
        spacing: root.spacing
        width: rowLayout.width

        RowLayout {
            id: rowLayout
            Label {
                text: "Name"
            }

            TextField {
                id: nameInput
                Layout.fillWidth: true
            }
        }
        GroupBox {
            anchors { left: parent.left; right: parent.right }
            title: "Lights assigned to this group"
            GridLayout {
                id: gridLayout
                anchors { left: parent.left; right: parent.right }

                columns: 2

                Repeater {
                    id: lightsRepeater

                    delegate: Row {
                        property alias checked: checkBox.checked
                        CheckBox {
                            id: checkBox
                        }
                        Label {
                            text: model.name
                        }
                    }
                }
            }
        }
        Row {
            anchors { left: parent.left; right: parent.right }
            layoutDirection: Qt.RightToLeft

            Button {
                text: "OK"
                enabled: nameInput.text
                onClicked: {
                    var lights = new Array
                    for (var i = 0; i < lightsRepeater.count; i++) {
                        if (lightsRepeater.itemAt(i).checked) {
                            lights.push(root.lights.get(i).id);
                        }
                    }
                    root.accepted(nameInput.text, lights);
                    root.destroy();
                }
            }
            Button {
                text: "Cancel"
                onClicked: {
                    root.rejected();
                    root.destroy();
                }
            }
        }
    }
}
