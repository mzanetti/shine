/*
 * Copyright 2013-2015 Michael Zanetti
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
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Popups 1.3
import Hue 0.1

Page {
    id: root
    title: "Scenes"

    property var lights: null
    property var scenes: null

    head {
        actions: [
            Action {
                iconName: "add"
                onTriggered: {
                    var popup = PopupUtils.open(Qt.resolvedUrl("EditSceneDialog.qml"), root, {title: i18n.tr("Create Scene"), lights: root.lights})
                    popup.accepted.connect(function(name, lightsList) {
                        var colors;
                        for (var i = 0; i < lightsList.length; i++) {
                            colors += root.lights.get(lightsList[i]).color
                        }

                        scenes.createScene(name, lightsList, colors);
                    })
                }
            }

        ]
    }

    Lights {
        id: allLights
    }

    ListView {
        anchors.fill: parent
        model: scenes

        delegate: ListItem {
            property var scene: scenes.get(index)
            trailingActions: ListItemActions {
                actions: [
                    Action {
                        iconName: "edit"
                        onTriggered: {
                            var checkedLights = [];
                            for (var i = 0; i < scene.lightsCount; i++) {
                                checkedLights.push(scene.light(i))
                            }

                            print("editing scene", scene.id)
                            var popup = PopupUtils.open(Qt.resolvedUrl("EditSceneDialog.qml"), root, {title: i18n.tr("Edit scene"), lights: root.lights, checkedLights: checkedLights, name: scene.name})
                            popup.accepted.connect(function(name, lightsList) {
                                print("updating scene", scene.id)
                                scenes.updateScene(scene.id, name, lightsList);
                            })
                        }
                    }
                ]
            }

            RowLayout {
                anchors.fill: parent
                anchors.margins: units.gu(1)
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Label {
                        Layout.fillWidth: true
                        text: model.name
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Repeater {
                            model: scene.lightsCount
                            Label {
                                text: allLights.findLight(scene.light(index)).name
                            }
                        }
                    }
                }
            }

            onClicked: {
                scenes.recallScene(index)
            }
        }
    }
}
