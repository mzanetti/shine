import QtQuick 2.4
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.1

Page {
    id: root
    property bool busy: false

    property ListModel toolButtons
    signal toolButtonClicked(int index)

    header: ToolBar {
        id: toolBar
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: ":"
                onClicked: mainMenu.open()
            }
            Label {
                text: root.title
                font.pixelSize: 20
                Layout.fillWidth: true
            }
            Repeater {
                model: toolButtons
                ToolButton {
                    text: model.text
                    onClicked: root.toolButtonClicked(index)
                }
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        visible: root.busy
        running: root.busy
    }
}
