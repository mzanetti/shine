import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import Hue 0.1

MainView {
    id: root
    width: units.gu(50)
    height: units.gu(75)

    automaticOrientation: true

    Component.onCompleted: {
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
    }

    Tabs {
        anchors.fill: parent
        visible: HueBridge.connectedBridge

        Tab {
            title: "Lights"
            LightsPage {

            }
        }

        Tab {
            title: "Groups"
            GroupsPage {

            }
        }
    }

    Component {
        id: loginComponent
        Dialog {
            id: connectDialog
            title: "Connect to Hue bridge"
            text: "Please press the button on the hue bridge and then \"Connect...\""

            Connections {
                target: HueBridge
                onCreateUserFailed: {
                    connectDialog.text = "Error creating user: " + errorMessage;
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
