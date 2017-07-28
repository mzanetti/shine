import QtQuick 2.4
import Ubuntu.Components 1.3

AbstractButton {
    id: root
    property alias source: image.source

    UbuntuShape {
        anchors.fill: parent

        source: Image {
            id: image
            anchors.fill: parent
        }
    }
}
