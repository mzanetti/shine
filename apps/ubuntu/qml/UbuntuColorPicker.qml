import QtQuick 2.0
import Ubuntu.Components 0.1
import Hue 0.1

Item {

    property alias color: colorPicker.color
    property alias pressed: colorPicker.pressed
    property alias touchDelegate: colorPicker.touchDelegate
    property alias showAll: colorPicker.showAll

    function calculateColor(x, y) {
        return colorPicker.calculateColor(x, y);
    }

    function calculateXy(color) {
        return colorPicker.calculateXy(color);
    }

    ShaderEffectSource {
        id: source
        anchors.centerIn: parent
        width: 1
        height: 1
        hideSource: true
        sourceItem: colorPicker
    }

    Shape {
        id: shape
        image: source

        anchors.fill: parent
    }

    ColorPicker {
        id: colorPicker
        anchors.fill: parent
    }

}
