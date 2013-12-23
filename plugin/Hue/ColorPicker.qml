import QtQuick 2.0

Item {
    id: root
    clip: true

    property color color

    function calculateXy(color) {
        var point = new Object;
        var brightness = Math.min(color.r, color.g, color.b);
        point.y = Math.round(root.height * brightness);
        var newColor = new Object;
        newColor.r = color.r - brightness;
        newColor.g = color.g - brightness;
        newColor.b = color.b - brightness;

        var sectionWidth = root.width / 6;

        var sections = [true, true, true];
        if (newColor.r > 0) {
            sections[1] = false;
        }
        if (newColor.g > 0) {
            sections[2] = false;
        }
        if (newColor.b > 0) {
            sections[0] = false;
        }
        if (sections[0]) {
            if (newColor.r > newColor.g) {
                point.x = sectionWidth * newColor.g;
            } else if (newColor.r == newColor.g) {
                point.x = sectionWidth
            } else {
                point.x = sectionWidth * 2 - newColor.r * sectionWidth;
            }
        } else if (sections[1]) {
            if (newColor.g > newColor.b) {
                point.x = sectionWidth * 2 + sectionWidth * newColor.b;
            } else if (newColor.g == newColor.b) {
                point.x = sectionWidth * 3;
            } else {
                point.x = sectionWidth * 4 - newColor.g * sectionWidth
            }
        } else {
            if (newColor.b > newColor.r) {
                point.x = sectionWidth * 4 + sectionWidth * newColor.r
            } else if (newColor.b == newColor.r) {
                point.x = sectionWidth * 5;
            } else {
                point.x = sectionWidth * 6 - newColor.b * sectionWidth;
            }
        }
        return point;
    }

    function calculateColor(x, y) {
        x = Math.min(Math.max(0, x), width - 1);
        y = Math.min(Math.max(0, y), height - 1);
        var color = root.color;
        var sectionWidth = root.width / 6;
        var section = Math.floor(x / sectionWidth)
        var sectionX = Math.floor(x % (sectionWidth - 0.00001)) // the - 0.00001 is to prevent a mismatch when rounding
        // sectionVal : 1.0 = sectionX : sectionWidth
        var sectionVal = (1.0 * sectionX / sectionWidth)

        // brightnessVal : 1.0 = mouseY : height
        var brightness = 1.0 * y / height


        switch (section) {
        case 0:
            // GradientStop { position: 0.0; color: "#ff0000" }
            color = Qt.rgba(1, sectionVal + brightness, brightness, 1)
            break;
        case 1:
            // GradientStop { position: 0.17; color: "#ffff00" }
            color = Qt.rgba(1 - sectionVal + brightness, 1, brightness, 1)
            break;
        case 2:
            // GradientStop { position: 0.33; color: "#00ff00" }
            color = Qt.rgba(brightness, 1, sectionVal + brightness, 1)
            break;
        case 3:
            // GradientStop { position: 0.5; color: "#00ffff" }
            color = Qt.rgba(brightness, 1 - sectionVal + brightness, 1, 1)
            break;
        case 4:
            // GradientStop { position: 0.66; color: "#0000ff" }
            color = Qt.rgba(sectionVal + brightness, brightness, 1, 1)
            break;
        case 5:
            // GradientStop { position: 0.83; color: "#ff00ff" }
            color = Qt.rgba(1, brightness, 1 - sectionVal + brightness, 1)
            break;
        }
        return color
    }

    Rectangle {
        height: parent.width
        width: parent.height
        anchors.centerIn: parent
        rotation: -90

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ff0000" }
            GradientStop { position: 0.17; color: "#ffff00" }
            GradientStop { position: 0.33; color: "#00ff00" }
            GradientStop { position: 0.5; color: "#00ffff" }
            GradientStop { position: 0.66; color: "#0000ff" }
            GradientStop { position: 0.83; color: "#ff00ff" }
            GradientStop { position: 1.0; color: "#ff0000" }
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 1.0; color: "white" }
        }
    }

    MouseArea {
        anchors.fill: parent
        onMouseXChanged: root.color = root.calculateColor(mouseX, mouseY);
        onMouseYChanged: root.color = root.calculateColor(mouseX, mouseY)
        preventStealing: true
    }
    Rectangle {
        height: root.height * .1
        width: height
        radius: height * .5
        color: root.color
        border.width: height * .2
        border.color: "black"
        property var coords: calculateXy(root.color);
        x: coords.x - height * .5
        y: coords.y - height * .5
    }
}

