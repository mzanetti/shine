import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Hue 0.1

Page {

    ListView {
        id: lightsListView
        anchors.fill: parent

        property var expandedItem: null

        model: Lights {
            id: lights
        }

        delegate: LightDelegate {
            id: delegateItem
            light: lights.get(index)

            states: [
                State {
                    name: "expanded"; when: lightsListView.expandedItem === delegateItem
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
                if (light.reachable) {
                    if (ListView.view.expandedItem == delegateItem) {
                        ListView.view.expandedItem = null;
                    } else {
                        ListView.view.expandedItem = delegateItem;
                    }
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

