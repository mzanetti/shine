import QtQuick 2.0
import Ubuntu.Components 0.1
import Hue 0.1

Page {
    UbuntuColorPicker {
        id: bigColorPicker
        anchors.fill: parent
        anchors.margins: units.gu(2)
        visible: root.orientation == "landscape"
        showAll: true

        touchDelegate: UbuntuShape {
            id: lightDelegate
            color: light && light.reachable ? (light.on ? "blue" : "gray") : "red"
            height: units.gu(5)
            width: units.gu(5)
            property var point: light ? bigColorPicker.calculateXy(light.color) : undefined
            radius: "medium"
            opacity: light && light.reachable ? 1 : .5
            Label {
                id: label
                color: "white"
                anchors.centerIn: parent
            }

            property var light
            property alias text: label.text
        }
    }
}
