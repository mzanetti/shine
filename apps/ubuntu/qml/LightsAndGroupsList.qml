import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3

ListView {
    id: root
    clip: true
    currentIndex: 0

    property var lights: null
    property var groups: null

    model: ListModel {
        id: lightsAndGroups
        Component.onCompleted: refresh()
        function refresh() {
            lightsAndGroups.clear();
            for (var i = 0; i < root.lights.count; i++) {
                lightsAndGroups.append({name: root.lights.get(i).name, id: root.lights.get(i).id, type: "light"})
            }
            for (var i = 0; i < root.groups.count; i++) {
                lightsAndGroups.append({name: root.groups.get(i).name, id: root.groups.get(i).id, type: "group"})
            }
        }
    }
    Connections {
        target: root.lights
        onCountChanged: lightsAndGroups.refresh();
    }
    delegate: ListItem {
        RowLayout {
            anchors.fill: parent
            anchors.margins: units.gu(2)
            Label {
                text: model.name
                Layout.fillWidth: true
            }
            Icon {
                name: "tick"
                visible: root.currentIndex == index
                Layout.preferredHeight: units.gu(2)
                Layout.preferredWidth: height
            }
        }
        onClicked: root.currentIndex = index
    }
}
