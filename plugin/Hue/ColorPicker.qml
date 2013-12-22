import QtQuick 2.0

Item {
    id: root

    property color color

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
}

