import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3

Page {
    id: root
    property var sensors: null

    ListView {
        model: root.sensors
        anchors.fill: parent
        delegate: Standard {
            text: model.name
        }
    }

}
