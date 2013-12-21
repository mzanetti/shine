import QtQuick 1.1
import com.nokia.meego 1.0
import Hue 0.1

Dialog {
    id: connectDialog
    title: Label {
        width: parent.width
        text: "Connect to Hue Bridge"
        font.pixelSize: 30
    }

    content: Label {
        id: mainLabel
        width: parent.width
        height: 100
        wrapMode: Text.WordWrap
        text: ""
        verticalAlignment: Text.AlignVCenter
    }

    Connections {
        target: hueBridge
        onUsernameChanged: {
            if (hueBridge.username) {
                connectDialog.accept()
            }
        }
        onCreateUserFailed: {
            mainLabel.text = "Error creating user: " + errorMessage;
            connectButton.text = "Try again!";
        }
    }

    buttons: ButtonRow {
        style: ButtonStyle { }
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
            id: connectButton
            text: "Connect..."
            onClicked: {
                hueBridge.createUser("Ubuntu touch", "abcdef1234567890")
                mainLabel.text = "Waiting for connection to establish...";
            }
        }
    }
}
