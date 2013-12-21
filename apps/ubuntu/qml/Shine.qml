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
            id: delegateItem
            clip: true
            property bool expanded: false

            states: [
                State {
                    name: "expanded"; when: delegateItem.expanded
                    PropertyChanges { target: delegateItem; height: units.gu(20) }
                }
            ]
            transitions: [
                Transition {
                    from: "*"; to: "*"
                    UbuntuNumberAnimation { properties: "height" }
                }

            ]

            Column {
                anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2) }
                spacing: units.gu(2)

                Row {
                    anchors {
                        left: parent.left
                        right: parent.right
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

                Slider {
                    anchors { left: parent.left; right: parent.right }
                    minimumValue: 0
                    maximumValue: 255
                    value: model.bri
                    onValueChanged: {
                        print("val changed")
                        lights.get(index).bri = value
                    }
                }
            }


            onClicked: {
                delegateItem.expanded = !delegateItem.expanded
            }
        }
    }
}
