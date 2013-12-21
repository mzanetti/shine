import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import Hue 0.1

MainView {
    width: units.gu(50)
    height: units.gu(75)

    ListView {
        anchors.fill: parent

        model: Lights {
            id: lights
        }

        delegate: Empty {

            Row {
                anchors {
                    left: parent.left
                    leftMargin: units.gu(2)
                    right: parent.right
                    rightMargin: units.gu(2)
                    top: parent.top
                }
                height: units.gu(6)

                Label {
                    width: parent.width - onOffSwitch.width
                    text: model.name
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id: onOffSwitch
                    checked: model.on
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        lights.get(index).on = checked;
                    }
                }
            }


        }
    }
}
