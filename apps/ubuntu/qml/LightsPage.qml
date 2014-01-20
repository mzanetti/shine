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
import Ubuntu.Components.Popups 0.1
import Hue 0.1

Page {
    id: root
    title: "Lights"

    property alias lights: lightsFilterModel.lights

    tools: ToolbarItems {
        ToolbarButton {
            text: "group"
            iconName: "remove"
            enabled: groupSelector.selectedIndex > 0
            onTriggered: {
                groups.deleteGroup(groups.get(groupSelector.selectedIndex).id)
                groupSelector.selectedIndex = 0;
            }
        }
        ToolbarButton {
            text: "group"
            iconName: "add"
            onTriggered: PopupUtils.open(addGroupComponent, root)
        }
    }

    Groups {
        id: groups
    }

    Column {
        anchors.fill: parent

        Row {
            anchors { left: parent.left; right: parent.right; margins: units.gu(2) }
            height: groupSelector.height + units.gu(2)
            spacing: units.gu(2)

            OptionSelector {
                id: groupSelector
                anchors { top: parent.top; margins: units.gu(1) }
                width: parent.width - groupSwitch.width - parent.spacing
                model: groups

                delegate: OptionSelectorDelegate {
                    text: name
                    height: units.gu(5)
                }

                onSelectedIndexChanged: {
                    lightsFilterModel.groupId = groups.get(selectedIndex).id
                }
            }
            Switch {
                id: groupSwitch
                anchors { top: parent.top; margins: units.gu(1) }
                iconSource: "image://theme/torch-off"
                checked: groups.get(groupSelector.selectedIndex).on
                visible: groups.count > 0
                onClicked: {
                    groups.get(groupSelector.selectedIndex).on = checked;
                }
            }
        }

        Divider {}

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

    Component {
        id: addGroupComponent
        ComposerSheet {
            title: "Add group"

            onConfirmClicked: {
                var lightsList = new Array;
                for (var i = 0; i < lightsCheckboxes.count; ++i) {
                    if (lightsCheckboxes.itemAt(i).checked) {
                        print("adding light", i)
                        lightsList.push(lights.get(i).id);
                        print("list is now", lightsList.length)
                    }
                }

                groups.createGroup(nameTextField.text, lightsList);
            }

            Column {
                anchors { fill: parent; margins: units.gu(2) }
                spacing: units.gu(1)

                Row {
                    anchors { left: parent.left; right: parent.right }
                    spacing: units.gu(1)

                    Label {
                        text: "Name"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    TextField {
                        id: nameTextField
                        width: parent.width - x
                    }
                }
                ThinDivider {}
                Column {
                    anchors { left: parent.left; right: parent.right }
                    spacing: units.gu(1)

                    Repeater {
                        id: lightsCheckboxes
                        model: root.lights
                        delegate: Row {
                            width: parent.width
                            spacing: units.gu(1)
                            property alias checked: checkBox.checked
                            CheckBox {
                                id: checkBox
                            }
                            Label {
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
        }
    }
}

