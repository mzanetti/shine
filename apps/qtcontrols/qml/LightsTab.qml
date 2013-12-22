import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

Tab {

    SplitView {
        anchors.fill: parent

        TableView {
            id: lightsView

            model: Lights {
                id: lights
            }

            TableViewColumn {
                role: "name"
                title: "Hue bulbs"
            }
        }

        Item {
            GridLayout {
                id: layout
                anchors.fill: parent
                anchors.margins: 20
                property var light: lights.get(lightsView.currentRow)
                columns: 2
                Label {
                    text: "Power: " + (layout.light && layout.light.on ? "On" : "Off")
                }
                Button {
                    text: "toggle"
                    Layout.fillWidth: true
                    onClicked: {
                        layout.light.on = !layout.light.on
                    }
                }
                Label {
                    text: "Brightness:"
                }
                Slider {
                    Layout.fillWidth: true
                    minimumValue: 0
                    maximumValue: 255
                    value: layout.light.bri
                    onValueChanged: {
                        print("val changed")
                        layout.light.bri = value
                    }
                }

                ColorPicker {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: 2
                    onColorChanged: {
                        layout.light.color = color;
                    }
                }
                Label {
                    text: "Effect:"
                }

                ComboBox {
                    id: effectCb
                    Layout.fillWidth: true
                    textRole: "name"
                    model: ListModel {
                        id: effectModel
                        ListElement { name: "No effect"; value: "none" }
                        ListElement { name: "Color loop"; value: "colorloop" }
                    }
                    currentIndex: getCurrentIndex(layout.light);
                    function getCurrentIndex(light){
                        for (var i = 0; i < effectModel.count; i++) {
                            if (effectModel.get(i).value == light.effect) {
                                return i;
                            }
                        }
                    }

                    Connections {
                        target: layout
                        onLightChanged: {
                            effectCb.currentIndex = effectCb.getCurrentIndex(layout.light);
                        }
                    }

                    onCurrentIndexChanged: {
                        layout.light.effect = effectModel.get(currentIndex).value;
                    }


                }
            }
        }
    }
}
