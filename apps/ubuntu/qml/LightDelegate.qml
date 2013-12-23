import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Hue 0.1

Empty {
    clip: true
    opacity: light.reachable ? 1 : .5

    property int expandedHeight: delegateColumn.height
    property var light

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
                spacing: units.gu(1)
                visible: opacity > 0

                Icon {
                    id: icon
                    height: parent.height - units.gu(2)
                    anchors.verticalCenter: parent.verticalCenter
                    width: height
                    name: light.reachable ? light.on ? "torch-on" : "torch-off" : "flash-off"
                }

                Label {
                    width: parent.width - onOffSwitch.width - icon.width - parent.spacing*2
                    text: light.name
                    anchors.verticalCenter: parent.verticalCenter
                }

                Switch {
                    id: onOffSwitch
                    checked: light.on
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        light.on = checked;
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
                    text: light.name
                }
                Button {
                    id: okButton
                    text: "OK"
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked: {
                        light.name = renameTextField.text
                        delegateItem.state = ""
                    }
                }
            }
        }

        Row {
            anchors { left: parent.left; right: parent.right }
            spacing: units.gu(1)
            Icon {
                height: brightnessSlider.height
                width: height
                name: "torch-off"
            }
            Slider {
                id: brightnessSlider
                width: parent.width - height * 2 - parent.spacing * 2
                minimumValue: 0
                maximumValue: 255
                value: light.bri
                onValueChanged: {
                    print("val changed")
                    light.bri = value
                }
            }
            Icon {
                height: brightnessSlider.height
                width: height
                name: "torch-on"
            }
        }


        UbuntuColorPicker {
            id: colorPicker
            anchors { left: parent.left; right: parent.right }
            height: width / 2
            color: light.color

            onColorChanged: {
                if (pressed) {
                    light.color = colorPicker.color;
                }
            }
        }

        OptionSelector {
            model: ListModel {
                id: effectModel
                ListElement { name: "No effect"; value: "none" }
                ListElement { name: "Color loop"; value: "colorloop" }
            }
            selectedIndex: {
                for (var i = 0; i < effectModel.count; i++) {
                    if (effectModel.get(i).value == light.effect) {
                        return i;
                    }
                }
            }

            onSelectedIndexChanged: {
                light.effect = effectModel.get(selectedIndex).value;
            }

            delegate: OptionSelectorDelegate {
                text: name
            }
        }
    }
}
