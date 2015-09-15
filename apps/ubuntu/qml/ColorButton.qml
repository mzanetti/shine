import QtQuick 2.4
import Ubuntu.Components 1.3

AbstractButton {
    property alias source: image.source

    UbuntuShape {

        source: Image {
            id: image
            anchors.fill: parent
        }
    }
}
