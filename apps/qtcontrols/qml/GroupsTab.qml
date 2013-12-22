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
            LightsControl {
                anchors.fill: parent
                anchors.margins: 20
                visible: groupsView.currentRow > -1
                light: groups.get(groupsView.cuurentRow)
            }
        }
    }
}
