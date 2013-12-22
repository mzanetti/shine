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
        id: tabView
        anchors.fill: parent

        LightsTab {
            title: "Lights"

        }

        GroupsTab {
            title: "Groups"
        }
    }
}
