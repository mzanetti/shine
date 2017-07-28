import QtQuick 2.4
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.1
import Hue 0.1

Page {
    id: root

    property var light: null
    property var schedules: null

    Binding {
        target: root.light
        property: "autoRefresh"
        value: true
    }

    header: ToolBar {
        RowLayout {
             anchors.fill: parent
             ToolButton {
                 text: "‹"
                 onClicked: pageStack.pop()
             }
             Label {
                 text: light.name
                 elide: Label.ElideRight
                 horizontalAlignment: Qt.AlignHCenter
                 verticalAlignment: Qt.AlignVCenter
                 Layout.fillWidth: true
             }
             ToolButton {
                 text: qsTr("⋮")
                 onClicked: menu.open()
             }
         }
    }

//    head {
//        actions: [
//            Action {
//                iconName: "alarm-clock"
//                onTriggered: {
//                    PopupUtils.open(Qt.resolvedUrl("CreateAlarmDialog.qml"), root, {light: root.light, schedules: root.schedules})
//                }
//            },
//            Action {
//                iconName: "camera-self-timer"
//                onTriggered: PopupUtils.open(Qt.resolvedUrl("CreateTimerDialog.qml"), root,  {light: root.light, schedules: root.schedules })
//            }
//        ]
//    }

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
        anchors.margins: 8 * (2)
        spacing: 8 * (2)

        RowLayout {
            Layout.fillWidth: true
            spacing: 8 * (1)
            Button {
                Layout.preferredHeight: 8 * (6)
                Layout.preferredWidth: height
                onClicked: light.on = false;
                Image {
                    anchors.fill: parent
                    anchors.margins: 8 * (1)
                    source: "torch-off"
                }
            }
            Slider {
                id: brightnessSlider
                Layout.fillWidth: true
                from: 0
                to: 255
                value: light ? light.bri : 0
                onValueChanged: {
                    light.bri = value
                }
            }
            Button {
                Layout.preferredHeight: 8 * (6)
                Layout.preferredWidth: height
                onClicked: light.on = true;
                Image {
                    anchors.fill: parent
                    anchors.margins: 8 * (1)
                    source: "torch-on"
                }
            }

        }

//        RowLayout {
//            spacing: 8 * (2)
//            Layout.fillWidth: true
//            Repeater {
//                model: LightRecipeModel {}
//                ColorButton {
//                    Layout.preferredHeight: width
//                    Layout.fillWidth: true
//                    source: "images/" + model.name + ".svg"

//                    onClicked: {
//                        light.color = model.color
//                        light.bri = model.bri
//                        light.on = true;
//                    }
//                }
//            }
//        }
        ColorPicker {
            id: colorPicker
            Layout.fillWidth: true
            Layout.fillHeight: true

            color: light ? light.color : "black"
            active: light ? (light.colormode == LightInterface.ColorModeHS || light.colormode == LightInterface.ColorModeXY) : false

            touchDelegate: Rectangle {
                height: 8 * (3)
                width: 8 * (3)
                color: "black"
            }

            onColorChanged: {
                if (pressed) {
                    print("light", light, "light.color", light.color, colorPicker.color)
                    light.color = colorPicker.color;
                }
            }
        }
        ColorPickerCt {
            id: colorPickerCt
            Layout.fillWidth: true
            Layout.preferredHeight: 8 * (8)

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
                width: 8 * (.5)
                color: "transparent"
                border.color: "black"
                border.width: units.dp(2)
            }
        }

//        ItemSelector {
//            id: effectSelector
//            Layout.fillWidth: true
//            model: ListModel {
//                id: effectModel
//                ListElement { name: "No effect"; value: "none" }
//                ListElement { name: "Color loop"; value: "colorloop" }
//            }
//            selectedIndex: findIndex()

//            function findIndex() {
//                if (!light) {
//                    return 0;
//                }

//                for (var i = 0; i < effectModel.count; i++) {
//                    if (effectModel.get(i).value == light.effect) {
//                        return i;
//                    }
//                }
//                return 0;
//            }

//            onSelectedIndexChanged: {
//                light.effect = effectModel.get(selectedIndex).value;
//            }

//            delegate: OptionSelectorDelegate {
//                text: name
//            }
//        }

    }

//    LightDelegate {
//        id: lightDelegate
//        width: parent.width
//        __isExpanded: true
//    }
}
