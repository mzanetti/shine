import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3

ListView {
    id: root
    clip: true
    currentIndex: -1

    property var lights: null
    property var groups: null

    function selectLight(id) {
        for (var i = 0; i < lightsAndGroups.count; i++) {
            if (lightsAndGroups.get(i).type == "light" && lightsAndGroups.get(i).id == id) {
                root.currentIndex = i;
            }
        }
    }

    function selectGroup(id) {
        for (var i = 0; i < lightsAndGroups.count; i++) {
            if (lightsAndGroups.get(i).type == "group" && lightsAndGroups.get(i).id == id) {
                root.currentIndex = i;
            }
        }
    }

    model: ListModel {
        id: lightsAndGroups
        Component.onCompleted: refresh()
        function refresh() {
            lightsAndGroups.clear();
            for (var i = 0; i < root.lights.count; i++) {
                lightsAndGroups.append({name: root.lights.get(i).name, id: root.lights.get(i).id, type: "light", isGroup: false})
            }
            for (var i = 0; i < root.groups.count; i++) {
                lightsAndGroups.append({name: root.groups.get(i).name, id: root.groups.get(i).id, type: "group", isGroup: true})
            }
        }
    }
    Connections {
        target: root.lights
        onCountChanged: lightsAndGroups.refresh();
    }
    delegate: ListItem {
        height: units.gu(5)
        RowLayout {
            anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2); topMargin: units.gu(1); bottomMargin: units.gu(1) }
            Label {
                text: model.name
                Layout.fillWidth: true
            }
            Icon {
                name: "tick"
                visible: root.currentIndex == index
                Layout.fillHeight: true
                Layout.preferredWidth: height
            }
        }
        onClicked: root.currentIndex = index
    }
}
