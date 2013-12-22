import QtQuick 2.1
import Hue 0.1
import QtQuick.Controls 1.0

ApplicationWindow {
    height: 1000
    width: 1000

    toolBar: ToolBar {
        ToolButton {
            iconName: "select"
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
                : HueBridge.foundHost && HueBridge.connectedBridge ? "Waiting for authentication"
                : HueBridge.connectedBridge ? "Connected to " + HueBridge.connectedBridge
                : "Searching for bridges..."
        }
    }

    Connections {
        target: HueBridge
        onCreateUserFailed: {
            statusLabel.text = errorMessage
        }
    }


    TabView {
        anchors.fill: parent

        LightsTab {
            title: "Lights"

        }

        GroupsTab {
            title: "Groups"
        }
    }

    NewGroupWindow {}
}
