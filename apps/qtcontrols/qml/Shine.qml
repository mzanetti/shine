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
import Hue 0.1
import QtQuick.Controls 1.0

ApplicationWindow {
    width: tabView.implicitWidth * 3
    minimumWidth: tabView.implicitWidth * 3
    height: tabView.implicitHeight * 3
    minimumHeight: tabView.implicitHeight * 3
    title: "Shine"

    toolBar: ToolBar {
        ToolButton {
            iconName: "add"
            text: "connect"
            onClicked: {
                HueBridge.createUser("shine", "1234567890abcdef")
            }
        }
    }

    statusBar: StatusBar {
        Label {
            id: statusLabel
            text: HueBridge.discoveryError ? "Error while discovering bridge... This will not work."
                : HueBridge.bridgeFound && !HueBridge.connectedBridge ? "Waiting for authentication"
                : HueBridge.connectedBridge ? "Connected to " + HueBridge.connectedBridge
                : "Searching for bridges..."
        }
    }

    Component.onCompleted: {
        HueBridge.apiKey = keystore.apiKey;
    }

    Connections {
        target: HueBridge
        onCreateUserFailed: {
            statusLabel.text = errorMessage
        }
        onBridgeFoundChanged: {
            print("found bridge", HueBridge.bridgeFound)
        }
        onApiKeyChanged: {
            keystore.apiKey = HueBridge.apiKey;
        }
    }

    Lights {
        id: lightsModel
    }

    TabView {
        id: tabView
        anchors.fill: parent

        LightsTab {
            title: "Lights"
            lights: lightsModel
        }

//        GroupsTab {
//            title: "Groups"
//        }

        Tab {
            title: "Colors"
            ColorPicker {
                id: colorPicker
                anchors.fill: parent
                lights: lightsModel
                touchDelegate: Rectangle {
                    height: colorPicker.height / 10
                    width: height
                    radius: height * .499
                    color: light ? (light.on ? "blue" : light.reachable ? "gray" : "red") : "transparent"
                    Label {
                        id: label
                        anchors.centerIn: parent
                        color: "white"
                    }

                    property var light
                    property alias text: label.text
                }
            }
        }
    }
}
