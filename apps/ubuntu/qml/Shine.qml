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
import QtQuick.Window 2.0
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Popups 1.3
import Hue 0.1

MainView {
    id: root
    width: units.gu(40)
    height: units.gu(70)

    property string orientation: width > height ? "landscape" : "portrait"

    applicationName: "com.ubuntu.developer.mzanetti.shine"
    theme.name: "Ubuntu.Components.Themes.SuruDark"

    Component.onCompleted: {
        HueBridge.apiKey = keystore.apiKey;

        if (HueBridge.discoveryError) {
            PopupUtils.open(errorComponent, root)
        } else if (HueBridge.bridgeFound && !HueBridge.connectedBridge){
            PopupUtils.open(loginComponent, root)
        }
    }

    Connections {
        target: HueBridge
        onBridgeFoundChanged: {
            if (!HueBridge.connectedBridge) {
                PopupUtils.open(loginComponent, root)
            }
        }
        onDiscoveryErrorChanged: {
            if (HueBridge.discoveryError) {
                PopupUtils.open(errorComponent, root)
            }
        }
        onApiKeyChanged: {
            keystore.apiKey = HueBridge.apiKey;
        }
    }


    Tabs {
        anchors.fill: parent
        visible: root.orientation == "portrait"

        Tab {
            title: "Lights"
            page: LightsPage {
                id: lightsPage
                lights: lights
            }
        }
        Tab {
            title: "Scenes"
            page: ScenesPage {
                id: scenesPage
                lights: lights
                scenes: scenes
            }
        }
        Tab {
            title: "Schedules"
            page: PageStack {
                id: pageStack
                Component.onCompleted: push(schedulesPage)
                SchedulesPage {
                    id: schedulesPage
                    lights: lights
                    schedules: schedules
                    scenes: scenes
                }
            }
        }
    }


    BigColorPicker {
        id: bigColorPicker
        anchors.fill: parent
        visible: root.orientation == "landscape"
        lights: lights
    }

    Lights {
        id: lights
    }

    Scenes {
        id: scenes
    }

    Schedules {
        id: schedules
    }

    Component {
        id: loginComponent
        Dialog {
            id: connectDialog
            title: "Connect to Hue bridge"
            text: "Please press the button on the Hue bridge and then \"Connect...\""

            Connections {
                target: HueBridge
                onCreateUserFailed: {
                    connectDialog.text = "Error authenticating to Hue bridge: " + errorMessage;
                    connectButton.text = "Try again!";
                }
                onConnectedBridgeChanged: {
                    if (HueBridge.connectedBridge) {
                        PopupUtils.close(connectDialog)
                    }
                }
            }

            Button {
                id: connectButton
                text: "Connect..."
                onClicked: {
                    HueBridge.createUser("Shine - Ubuntu touch", "abcdef1234567890")
                    connectDialog.text = "Waiting for the connection to establish..."
                }
            }
            Button {
                text: "Quit"
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }

    Component {
        id: errorComponent
        Dialog {
            id: errorDialog
            title: "Error discovering bridges"
            text: "Could not start discovery for bridges. This won't work."
            Button {
                text: "Quit"
                onClicked: Qt.quit();
            }
        }
    }

    Column {
        anchors { left: parent.left; right: parent.right }
        anchors.verticalCenter: parent.verticalCenter
        spacing: units.gu(5)
        visible: searchingSpinner.running
        ActivityIndicator {
            id: searchingSpinner
            anchors.horizontalCenter: parent.horizontalCenter
            height: units.gu(5)
            width: height
            running: !HueBridge.discoveryError && !HueBridge.bridgeFound
        }
        Label {
            anchors { left: parent.left; right: parent.right }
            text: "Searching for Hue bridges..."
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
