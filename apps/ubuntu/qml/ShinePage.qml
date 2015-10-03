import QtQuick 2.4
import Ubuntu.Components 1.3

Page {
    id: root
    property bool busy: false

    ActivityIndicator {
        anchors.centerIn: parent
        visible: root.busy
        running: root.busy
    }
}
