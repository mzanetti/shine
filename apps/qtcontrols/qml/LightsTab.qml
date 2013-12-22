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
            LightsControl {
                anchors.fill: parent
                anchors.margins: 20
                visible: lightsView.currentRow > -1
                light: lights.get(lightsView.cuurentRow)
            }
        }
    }
}
