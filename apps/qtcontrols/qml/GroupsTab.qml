import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

Tab {

    SplitView {
        Item {
            width: 200
            GridLayout {
                id: layout2
                anchors.fill: parent
                anchors.margins: 20
                columns: 2
                TableView {
                    id: groupsView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.columnSpan: 2

                    model: Groups {
                        id: groups
                    }

                    TableViewColumn {
                        role: "name"
                        title: "Hue groups"
                    }
                }
                Button {
                    id: deleteButton
                    Layout.fillWidth: true
                    Layout.columnSpan: 2
                    property var group: groups.get(groupsView.currentRow)
                    text: "Delete Group"
                    onClicked: {
                        groups.deleteGroup(deleteButton.group.id);
                    }
                }
                TextField {
                    id: nameInput
                    text: "New Group"
                    Layout.fillWidth: true
                }
                Button {
                    id: addButton
                    Layout.fillWidth: true
                    text: "Add"
                    onClicked: {
                        if (nameInput.text.length) {
                            groups.createGroup(nameInput.text);
                            nameInput.text = ""
                        }
                    }
                }
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
