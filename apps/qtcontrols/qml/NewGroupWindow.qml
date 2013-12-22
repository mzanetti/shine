import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Hue 0.1

ApplicationWindow {
    id: root
    width: mainColumn.implicitWidth * 1.5
    minimumWidth: mainColumn.implicitWidth
    height: mainColumn.implicitHeight
    minimumHeight: mainColumn.implicitHeight
    title: "Add Group"

    property int spacing: rowLayout.height / 4

    signal accepted(string name, var lights)
    signal rejected()

    Component.onDestruction: print("destroying window")

    ColumnLayout {
        id: mainColumn
        anchors.fill: parent
        spacing: root.spacing
        width: rowLayout.width

        RowLayout {
            id: rowLayout
            Label {
                text: "Name"
            }

            TextField {
                id: nameInput
                Layout.fillWidth: true
            }
        }
        GroupBox {
            anchors { left: parent.left; right: parent.right }
            title: "Lights assigned to this group"
            GridLayout {
                id: gridLayout
                anchors { left: parent.left; right: parent.right }

                columns: 2

                Repeater {
                    id: lightsRepeater
                    model: Lights {}

                    delegate: Row {
                        property alias checked: checkBox.checked
                        CheckBox {
                            id: checkBox
                        }
                        Label {
                            text: model.name
                        }
                    }
                }
            }
        }
        Row {
            anchors { left: parent.left; right: parent.right }
            layoutDirection: Qt.RightToLeft

            Button {
                text: "OK"
                enabled: nameInput.text
                onClicked: {
                    var lights = new Array
                    for (var i = 0; i < lightsRepeater.count; i++) {
                        lights[i] = lightsRepeater.itemAt(i).checked;
                    }
                    root.accepted(nameInput.text, lights);
                    root.destroy();
                }
            }
            Button {
                text: "Cancel"
                onClicked: {
                    root.rejected();
                    root.destroy();
                }
            }
        }
    }
}
