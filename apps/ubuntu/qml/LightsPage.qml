import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Hue 0.1

Page {

    ListView {
        anchors.fill: parent

        model: Lights {
            id: lights
        }

        delegate: LightDelegate {
            id: delegateItem
            light: lights.get(index)

            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: delegateItem; height: delegateItem.expandedHeight + units.gu(2) }
                },
                State {
                    name: "rename"
                    PropertyChanges { target: mainRow; opacity: 0 }
                    PropertyChanges { target: renameRow; opacity: 1 }
                }

            ]
            transitions: [
                Transition {
                    from: "*"; to: "*"
                    UbuntuNumberAnimation { properties: "height" }
                    UbuntuNumberAnimation { properties: "opacity" }
                }
            ]



            onClicked: {
                if (delegateItem.state == "expanded") {
                    delegateItem.state = ""
                } else {
                    delegateItem.state = "expanded"
                }
            }
            onPressAndHold: {
                if (delegateItem.state == "rename") {
                    delegateItem.state = ""
                } else {
                    delegateItem.state = "rename"
                }
            }
        }
    }
}

