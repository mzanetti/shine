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
import QtQuick.Window 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import Hue 0.1

MainView {
    id: root
    width: units.gu(50)
    height: units.gu(75)

//    applicationName: "com.ubuntu.developer.mzanetti.shine"

    automaticOrientation: true
    property string orientation: pageStack.width > pageStack.height ? "landscape" : "portrait"

    onOrientationChanged: {
        if (orientation == "portrait") {
            pageStack.pop();
//            pageStack.push(Qt.resolvedUrl("MainTabs.qml"))
            pageStack.push(lightsPage)
        } else {
            pageStack.pop();
            pageStack.push(bigColorPicker)
        }
    }

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

    PageStack {
        id: pageStack
    }

    LightsPage {
        id: lightsPage
        visible: false
        lights: lights
    }

    BigColorPicker {
        id: bigColorPicker
        visible: false
        lights: lights
    }

    Lights {
        id: lights
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
                    HueBridge.createUser("Ubuntu touch", "abcdef1234567890")
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
