import QtQuick 2.0
import Ubuntu.Components 0.1
import Hue 0.1

Page {
    UbuntuColorPicker {
        id: bigColorPicker
        anchors.fill: parent
        anchors.margins: units.gu(2)
        visible: root.orientation == "landscape"

        property var draggedItem
        MouseArea {
            id: dragArea
            anchors.fill: parent
            drag.minimumX: 0
            drag.maximumX: width - units.gu(3)
            drag.minimumY: 0
            drag.maximumY: height - units.gu(3)
            onPressed: {
                for (var i = 0; i < lightsDraghandleRepeater.count; i++) {
                    var item = lightsDraghandleRepeater.itemAt(i);
                    if (mouseX > item.x && mouseX < (item.x + item.width) && mouseY > item.y && mouseY < (item.y + item.height)) {
                        print("clicked on item");
                        dragArea.drag.target = item;
                        bigColorPicker.draggedItem = lights.get(i)
                    } else {
                        print("clicked not on item")
                    }
                }
            }
            onPositionChanged: {
                if (bigColorPicker.draggedItem) {
                    bigColorPicker.draggedItem.color = bigColorPicker.calculateColor(mouseX, mouseY);
                }
            }

            onReleased: {
                dragArea.drag.target = undefined;
                bigColorPicker.draggedItem = undefined;
            }
        }

        Repeater {
            id: lightsDraghandleRepeater
            model: Lights {
                id: lights
            }

            delegate: UbuntuShape {
                color: "blue"
                height: units.gu(3)
                width: units.gu(3)
                x: units.gu(5) * (index) + units.gu(2)
                y: parent.height - units.gu(5)
                Label {
                    text: index + 1
                    color: "white"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
