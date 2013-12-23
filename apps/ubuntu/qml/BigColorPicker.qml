import QtQuick 2.0
import Ubuntu.Components 0.1
import Hue 0.1

Page {
    UbuntuColorPicker {
        id: bigColorPicker
        anchors.fill: parent
        anchors.margins: units.gu(2)
        visible: root.orientation == "landscape"
        showIndicator: false

        property var draggedItem
        property var draggedLight

        property int itemSize: units.gu(5)
        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.minimumX: 0
            drag.maximumX: width - bigColorPicker.itemSize
            drag.minimumY: 0
            drag.maximumY: height - bigColorPicker.itemSize
            onPressed: {
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

                        dragArea.drag.target = dndItem;
                        dndItem.x = item.x;
                        dndItem.y = item.y;
                        dndItem.text = i + 1;
                        bigColorPicker.draggedLight = lights.get(i)
                        bigColorPicker.draggedItem = item;
                    }
                }
            }
            onPositionChanged: {
                if (bigColorPicker.draggedItem) {
                    bigColorPicker.draggedLight.color = bigColorPicker.calculateColor(mouseX, mouseY);
                }
            }

            onReleased: {
                bigColorPicker.draggedItem = undefined;
                bigColorPicker.draggedLight = undefined;
                dragArea.drag.target = undefined;

            }
        }

        Repeater {
            id: lightsDraghandleRepeater
            model: Lights {
                id: lights
            }

            delegate: UbuntuShape {
                id: lightDelegate
                color: lights.get(index).on ? "blue" : lights.get(index).reachable ? "gray" : "red"
                height: bigColorPicker.itemSize
                width: bigColorPicker.itemSize
                property var point: bigColorPicker.calculateXy(lights.get(index).color)
                x: Math.max(0, Math.min(point.x - width * .5, parent.width - bigColorPicker.itemSize))
                y: Math.max(0, Math.min(point.y - height * .5, parent.height - bigColorPicker.itemSize))
                visible: bigColorPicker.draggedItem != lightDelegate
                radius: "medium"
                opacity: lights.get(index).reachable ? 1 : .5
                Label {
                    text: index + 1
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
        UbuntuShape {
            id: dndItem
            color: "blue"
            radius: "medium"
            height: bigColorPicker.itemSize
            width: bigColorPicker.itemSize
            visible: bigColorPicker.draggedItem != undefined
            property alias text: dndItemLabel.text
            Label {
                id: dndItemLabel
                color: "white"
                anchors.centerIn: parent
            }
        }
    }
}
