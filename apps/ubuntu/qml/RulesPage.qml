import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3

Page {
    id: root
    property var rules: null

    ListView {
        model: root.rules
        anchors.fill: parent
        delegate: Standard {
            text: model.name
        }
    }

}
