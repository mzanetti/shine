import QtQuick 2.0
import Hue 0.1

Item {
    id: root
    clip: true

    property color color
    property bool pressed: mouseArea.pressed
    property Component touchDelegate
    property bool showAll: false

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
        id: mouseArea
        anchors.fill: parent
        preventStealing: true

        drag.minimumX: 0
        drag.maximumX: width - dndItem.width
        drag.minimumY: 0
        drag.maximumY: height - dndItem.height

        property var draggedItem
        property var draggedLight

        onPressed: {
            if (showAll) {
                for (var i = 0; i < lightsDraghandleRepeater.count; i++) {
                    var item = lightsDraghandleRepeater.itemAt(i);
                    if (mouseX > item.x && mouseX < (item.x + item.width) && mouseY > item.y && mouseY < (item.y + item.height)) {
                        if (!lights.get(i).reachable) {
                            print("light not reachable... not moving");
                            return;
                        }
                        if (!lights.get(i).on) {
                            lights.get(i).on = true;
                            print("light was off. turning on");
                        }

                        mouseArea.drag.target = dndItem;
                        dndItem.x = item.x;
                        dndItem.y = item.y;

                        if (dndItem.item) {
                            if (dndItem.item.hasOwnProperty("text")) dndItem.item.text = i + 1;
                            if (dndItem.item.hasOwnProperty("light")) dndItem.item.light = lights.get(i);
                        }

                        mouseArea.draggedLight = lights.get(i)
                        mouseArea.draggedItem = item;
                    }
                }
            } else {
                mouseArea.drag.target = dndItem;
                dndItem.x = touchDelegateLoader.x;
                dndItem.y = touchDelegateLoader.y;
                mouseArea.draggedItem = touchDelegateLoader;
            }
        }
        onPositionChanged: {
            root.color = root.calculateColor(mouseX, mouseY);

            if (mouseArea.draggedItem) {
                mouseArea.draggedLight.color = root.calculateColor(mouseX, mouseY);
            }
        }

        onReleased: {
            mouseArea.draggedItem = undefined;
            mouseArea.draggedLight = undefined;
            mouseArea.drag.target = undefined;

        }

    }

    Loader {
        id: touchDelegateLoader
        property var point: calculateXy(root.color);
        x: item ? Math.max(0, Math.min(point.x - width * .5, parent.width - item.width)) : 0
        y: item ? Math.max(0, Math.min(point.y - height * .5, parent.height - item.height)) : 0
        sourceComponent: root.showAll ? undefined : root.touchDelegate
        visible: mouseArea.draggedItem != touchDelegateLoader
    }

    Lights {
        id: lights
    }
    Repeater {
        id: lightsDraghandleRepeater
        model: root.showAll ? lights : undefined

        delegate: Loader {
            id: lightDelegate
            sourceComponent: root.touchDelegate
            property var point: root.calculateXy(lights.get(index).color)
            x: item ? Math.max(0, Math.min(point.x - width * .5, parent.width - item.width)) : 0
            y: item ? Math.max(0, Math.min(point.y - height * .5, parent.height - item.height)) : 0
            visible: mouseArea.draggedItem != lightDelegate
            onLoaded: {
                if (item.hasOwnProperty("text")) item.text = index + 1;
                item.light = lights.get(index)
            }
        }
    }

    Loader {
        id: dndItem
        sourceComponent: root.touchDelegate
        visible: mouseArea.draggedItem != undefined
    }
}

