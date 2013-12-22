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
                    Layout.fillWidth: true
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
                    Layout.fillWidth: true
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
                }
            }
        }
    }
}
