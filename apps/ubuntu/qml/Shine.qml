import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import Hue 0.1

MainView {
    id: root
    width: units.gu(50)
    height: units.gu(75)

    Component.onCompleted: {
        if (!HueBridge.username) {
            PopupUtils.open(loginComponent, root)
        }
    }

    Tabs {
        anchors.fill: parent

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
                onUsernameChanged: {
                    if (HueBridge.username) {
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
}
