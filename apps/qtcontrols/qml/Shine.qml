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
            text: HueBridge.username ? ("username:" + HueBridge.username) : "Please connect to a HueBridge"
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

        Tab {
            title: "Groups"
//            GroupsPage {

//            }
        }
    }



//    Column {
//        anchors.fill: parent

//        Rectangle {
//            width: parent.width
//            height: 100
//            color: "blue"
//            Text {
//                anchors.centerIn: parent
//                text: "current user:" + HueBridge.username
//            }
//        }

//        Rectangle {
//            width: parent.width
//            height: 100
//            color: "khaki"
//            Text {
//                anchors.centerIn: parent
//                text: "Create user"
//            }
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    HueBridge.createUser("testdevice", "micha1234567")
//                }
//            }
//        }

//        ListView {
//            width: parent.width
//            height: 300

//            model: Lights {

//            }

//            delegate: Item {
//                width: parent.width
//                height: 100
//                Text {
//                    anchors.centerIn: parent
//                    text: model.name
//                }
//            }
//        }

//    }

}
