import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

Tab {

    SplitView {
        anchors.fill: parent

        TableView {
            id: groupsView

            model: Groups {
                id: groups
            }

            TableViewColumn {
                role: "name"
                title: "Hue groups"
            }
        }

        Item {
            GridLayout {
                id: layout
                anchors.fill: parent
                anchors.margins: 20
                property var group: groups.get(groupsView.currentRow)
                columns: 2
                Label {
                    text: "Power: " + (layout.group && layout.group.on ? "On" : "Off")
                }
                Button {
                    text: "toggle"
                    Layout.fillWidth: true
                    onClicked: {
                        layout.group.on = !layout.group.on
                    }
                }
                Label {
                    text: "Brightness:"
                }
                Slider {
                    Layout.fillWidth: true
                    minimumValue: 0
                    maximumValue: 255
                    value: layout.group.bri
                    onValueChanged: {
                        print("val changed")
                        layout.group.bri = value
                    }
                }

                ColorPicker {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: 2
                    onColorChanged: {
                        layout.group.color = color;
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
                    currentIndex: getCurrentIndex(layout.group);
                    function getCurrentIndex(group){
                        for (var i = 0; i < effectModel.count; i++) {
                            if (effectModel.get(i).value == group.effect) {
                                return i;
                            }
                        }
                    }

                    Connections {
                        target: layout
                        onGroupChanged: {
                            effectCb.currentIndex = effectCb.getCurrentIndex(layout.group);
                        }
                    }

                    onCurrentIndexChanged: {
                        layout.group.effect = effectModel.get(currentIndex).value;
                    }


                }
            }
        }
    }
}
