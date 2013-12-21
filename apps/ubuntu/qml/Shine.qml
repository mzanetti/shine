import QtQuick 2.0
import Hue 0.1

Item {
    width: 500
    height: 500

    Column {
        anchors.fill: parent

        Rectangle {
            width: parent.width
            height: 100
            color: "blue"
            Text {
                anchors.centerIn: parent
                text: "current user:" + HueBridge.username
            }
        }

        Rectangle {
            width: parent.width
            height: 100
            color: "khaki"
            Text {
                anchors.centerIn: parent
                text: "Create user"
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    HueBridge.createUser("testdevice", "micha1234567")
                }
            }
        }

        ListView {
            width: parent.width
            height: 300

            model: Lights {

            }

            delegate: Item {
                width: parent.width
                height: 100
                Text {
                    anchors.centerIn: parent
                    text: model.name + " " + (model.on ? "On" : "off")
                }
            }
        }

    }

}
