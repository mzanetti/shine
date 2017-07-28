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
import QtQuick.Controls 2.1
import Hue 0.1

ShinePage {
    id: root
    title: "Scenes"
    busy: scenes && scenes.count === 0 && scenes.busy

    property var lights: null
    property var scenes: null
    property var schedules: null

    Label {
        anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }
        wrapMode: Text.WordWrap
        text: "No scenes created. You can create scenes by using the + button in the header."
//        fontSize: "x-large"
        horizontalAlignment: Text.AlignHCenter
        visible: scenes.count === 0 && !root.busy
        z: 2
    }

//    head {
//        actions: [
//            Action {
//                iconName: "settings"
//                onTriggered: PopupUtils.open(sceneSettingsComponent, root)
//            },
//            Action {
//                iconName: "add"
//                onTriggered: {
//                    var popup = PopupUtils.open(Qt.resolvedUrl("EditSceneDialog.qml"), root, {title: i18n.tr("Create Scene"), lights: root.lights})
//                    popup.accepted.connect(function(name, lightsList) {
//                        scenes.createScene(name, lightsList);
//                    })
//                }
//            }

//        ]
//    }

    ScenesFilterModel {
        id: scenesFilterModel
        scenes: root.scenes
        hideOtherApps: settings.hideScenesByOtherApps
    }

    ListView {
        anchors.fill: parent
        model: scenesFilterModel

        delegate: ItemDelegate {
            property var scene: scenesFilterModel.get(index)
//            trailingActions: ListItemActions {
//                actions: [
//                    Action {
//                        iconName: "alarm-clock"
//                        onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateAlarmDialog.qml"), root, {scene: scenesFilterModel.get(index), schedules: root.schedules } )
//                    },
//                    Action {
//                        iconName: "edit"
//                        onTriggered: {
//                            var checkedLights = [];
//                            for (var i = 0; i < scene.lightsCount; i++) {
//                                checkedLights.push(scene.light(i))
//                            }

//                            var popup = PopupUtils.open(Qt.resolvedUrl("EditSceneDialog.qml"), root, {title: i18n.tr("Edit scene"), lights: root.lights, checkedLights: checkedLights, name: scene.name})
//                            popup.accepted.connect(function(name, lightsList) {
//                                scenes.updateScene(scene.id, name, lightsList);
//                            })
//                        }
//                    }
//                ]
//            }

            RowLayout {
                anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2) }
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
                            model: root.lights.count > 0 && scene ? scene.lightsCount : 0
                            Label {
                                text: scene ? lights.findLight(scene.light(index)).name : ""
                            }
                        }
                    }
                }
            }

            onClicked: {
                scenes.recallScene(scene.id)
            }
        }
    }

    Component {
        id: sceneSettingsComponent
        Dialog {
            id: sceneSettingsDialog
            title: "Scene settings"
            RowLayout {
                CheckBox {
                    id: showOtherApps
                    checked: !settings.hideScenesByOtherApps
                }
                Label {
                    color: "black"
                    text: "Show scenes created by other apps"
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
            }
            Button {
                text: "OK"
                onClicked: {
                    settings.hideScenesByOtherApps = !showOtherApps.checked
                    PopupUtils.close(sceneSettingsDialog)
                }
            }
        }
    }
}
