import QtQuick 2.0
import Ubuntu.Components 0.1
import Hue 0.1

Item {

    property alias color: colorPicker.color
    function calculateColor(x, y) {
        return colorPicker.calculateColor(x, y);
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
