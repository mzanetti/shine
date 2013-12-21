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

            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: delegateItem; height: delegateColumn.height + units.gu(2) }
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

            Column {
                id: delegateColumn
                anchors { left: parent.left; right: parent.right; leftMargin: units.gu(2); rightMargin: units.gu(2) }
                spacing: units.gu(2)
                height: childrenRect.height

                Item {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: units.gu(6)
                    Row {
                        id: mainRow
                        anchors.fill: parent
                        spacing: units.gu(2)
                        visible: opcity > 0

                        Label {
                            width: parent.width - onOffSwitch.width - parent.spacing
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
                    Row {
                        id: renameRow
                        anchors.fill: parent
                        spacing: units.gu(2)
                        height: units.gu(6)
                        visible: opacity > 0
                        opacity: 0

                        TextField {
                            id: renameTextField
                            width: parent.width - okButton.width - parent.spacing
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.name
                        }
                        Button {
                            id: okButton
                            text: "OK"
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                lights.get(index).name = renameTextField.text
                                delegateItem.state = ""
                            }
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
                ColorPicker {
                    id: colorPicker

                    onColorChanged: {
                        lights.get(index).color = colorPicker.color;
                    }

                }
                Rectangle {
                    width: parent.width
                    height: units.gu(2)
                    color: colorPicker.color
                }

                Label {
                    text: "Effect"
                }

                OptionSelector {
                    model: ListModel {
                        id: effectModel
                        ListElement { name: "None"; value: "none" }
                        ListElement { name: "Color loop"; value: "colorloop" }
                    }
                    selectedIndex: {
                        for (var i = 0; i < effectModel.count; i++) {
                            if (effectModel.get(i).value == lights.get(index).effect) {
                                return i;
                            }
                        }
                    }

                    onSelectedIndexChanged: {
                        lights.get(index).effect = effectModel.get(selectedIndex).value;
                    }

                    delegate: OptionSelectorDelegate {
                        text: name
                    }
                }
            }


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
