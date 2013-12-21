import QtQuick 1.1
import com.nokia.meego 1.0
import Hue 0.1

Page {
    id: root
    tools: commonTools

    QtObject {
        id: units

        function gu(val) {
            return val * 10;
        }
    }

    ListView {
        anchors.fill: parent

        model: Lights {
            id: lights
        }

        delegate: Rectangle {
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#222222" }
                GradientStop { position: 1.0; color: "#111111" }
            }

            id: delegateItem
            width: parent.width
            height: units.gu(8)
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
                    NumberAnimation { properties: "height"; duration: 150 }
                    NumberAnimation { properties: "opacity"; duration: 150 }
                }
            ]

            MouseArea {
                anchors.fill: parent
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
                        visible: opacity > 0

                        Label {
                            width: parent.width - onOffSwitch.width - parent.spacing
                            text: model.name
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Switch {
                            id: onOffSwitch
                            checked: model.on
                            anchors.verticalCenter: parent.verticalCenter
                            onCheckedChanged: {
                                lights.get(index).on = checked;
                            }

                            Component.onCompleted: {
                                print("switch created. light is on", model.on);
                            }
                            Connections {
                                target: lights.get(index)
                                onOnChanged: {
                                    print("lightbulb on changed", lights.get(index).on);
                                }
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
                            width: units.gu(8)
                            onClicked: {
                                lights.get(index).name = renameTextField.text
                                delegateItem.state = ""
                            }
                        }
                    }
                }

                Row {
                    anchors { left: parent.left; right: parent.right }
                    spacing: units.gu(1)
                    Image {
                        height: brightnessSlider.height
                        width: height
//                        source: "torch-off"
                    }
                    Slider {
                        id: brightnessSlider
                        width: parent.width - height * 2 - parent.spacing * 2
                        minimumValue: 0
                        maximumValue: 255
                        value: model.bri
                        onValueChanged: {
                            print("val changed")
                            lights.get(index).bri = value
                        }
                    }
                    Image {
                        height: brightnessSlider.height
                        width: height
//                        source: "image://theme/torch-on"
                    }
                }


                ColorPickerQtQuick1 {
                    id: colorPicker
                    anchors { left: parent.left; right: parent.right }
                    height: width / 2

                    onColorChanged: {
                        lights.get(index).color = colorPicker.color;
                    }
                }

//                OptionSelector {
//                    model: ListModel {
//                        id: effectModel
//                        ListElement { name: "No effect"; value: "none" }
//                        ListElement { name: "Color loop"; value: "colorloop" }
//                    }
//                    selectedIndex: {
//                        for (var i = 0; i < effectModel.count; i++) {
//                            if (effectModel.get(i).value == lights.get(index).effect) {
//                                return i;
//                            }
//                        }
//                    }

//                    onSelectedIndexChanged: {
//                        lights.get(index).effect = effectModel.get(selectedIndex).value;
//                    }

//                    delegate: OptionSelectorDelegate {
//                        text: name
//                    }
//                }
            }
        }
    }
}
