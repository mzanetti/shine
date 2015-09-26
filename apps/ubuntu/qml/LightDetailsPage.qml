import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3
import Hue 0.1

Page {
    id: root
    title: light.name

    property var light: null
    property var schedules: null

    Binding {
        target: root.light
        property: "autoRefresh"
        value: true
    }

    head {
        actions: [
            Action {
                iconName: "alarm-clock"
                onTriggered: {
                    PopupUtils.open(Qt.resolvedUrl("CreateAlarmDialog.qml"), root, {light: root.light, schedules: root.schedules})
                }
            },
            Action {
                iconName: "camera-self-timer"
                onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateTimerDialog.qml"), root,  {light: root.light, schedules: root.schedules })
            }
        ]
    }

    Connections {
        target: root.light
        onStateChanged: {
            brightnessSlider.value = root.light.bri;
            effectSelector.selectedIndex = effectSelector.findIndex();
            if (!colorPicker.pressed || !colorPicker.active) {
                colorPicker.color = root.light.color;
            }
            colorPicker.active = light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false
            if (!colorPickerCt.pressed) {
                colorPickerCt.ct = root.light.ct;
            }
            colorPickerCt.active = !colorPicker.active
        }
        onNameChanged: {
            nameLabel.text = root.light.name;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: units.gu(2)
        spacing: units.gu(2)

        RowLayout {
            Layout.fillWidth: true
            spacing: units.gu(1)
            Button {
                Layout.preferredHeight: units.gu(6)
                Layout.preferredWidth: height
                onClicked: light.on = false;
                color: "white"
                Icon {
                    anchors.fill: parent
                    anchors.margins: units.gu(1)
                    name: "torch-off"
                }
            }
            Slider {
                id: brightnessSlider
                Layout.fillWidth: true
                minimumValue: 0
                maximumValue: 255
                value: light ? light.bri : 0
                onValueChanged: {
                    light.bri = value
                }
            }
            Button {
                Layout.preferredHeight: units.gu(6)
                Layout.preferredWidth: height
                onClicked: light.on = true;
                color: "white"
                Icon {
                    anchors.fill: parent
                    anchors.margins: units.gu(1)
                    name: "torch-on"
                }
            }

        }

        RowLayout {
            spacing: units.gu(2)
            Layout.fillWidth: true
            Repeater {
                model: LightRecipeModel {}
                ColorButton {
                    Layout.preferredHeight: width
                    Layout.fillWidth: true
                    source: "images/" + model.name + ".svg"

                    onClicked: {
                        light.color = model.color
                        light.bri = model.bri
                        light.on = true;
                    }
                }
            }
        }
        UbuntuColorPicker {
            id: colorPicker
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: light ? light.color : "black"
            active: light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false

            touchDelegate: UbuntuShape {
                height: units.gu(3)
                width: units.gu(3)
                backgroundColor: "black"
            }

            onColorChanged: {
                if (pressed) {
                    print("light", light, "light.color", light.color, colorPicker.color)
                    light.color = colorPicker.color;
                }
            }
        }
        UbuntuColorPickerCt {
            id: colorPickerCt
            Layout.fillWidth: true
            Layout.preferredHeight: units.gu(8)

            height: width / 6
            ct: light ? light.ct : minCt
            active: light && light.colormode == LightInterface.ColorModeCT

            onCtChanged: {
                if (pressed) {
                    light.ct = colorPickerCt.ct;
                }
            }

            touchDelegate: Rectangle {
                height: colorPickerCt.height
                width: units.gu(.5)
                color: "transparent"
                border.color: "black"
                border.width: units.dp(2)
            }
        }

        ItemSelector {
            id: effectSelector
            Layout.fillWidth: true
            model: ListModel {
                id: effectModel
                ListElement { name: "No effect"; value: "none" }
                ListElement { name: "Color loop"; value: "colorloop" }
            }
            selectedIndex: findIndex()

            function findIndex() {
                if (!light) {
                    return 0;
                }

                for (var i = 0; i < effectModel.count; i++) {
                    if (effectModel.get(i).value == light.effect) {
                        return i;
                    }
                }
                return 0;
            }

            onSelectedIndexChanged: {
                light.effect = effectModel.get(selectedIndex).value;
            }

            delegate: OptionSelectorDelegate {
                text: name
            }
        }

    }

//    LightDelegate {
//        id: lightDelegate
//        width: parent.width
//        __isExpanded: true
//    }
}
