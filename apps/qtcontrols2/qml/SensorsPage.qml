import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Hue 0.1

ShinePage {
    id: root
    title: "Switches"
    busy: sensors && sensors.count === 0 && sensors.busy

    property var sensors: null
    property var rules: null
    property var lights: null
    property var groups: null
    property var scenes: null

    SensorsFilterModel {
        id: filterModel
        sensors: root.sensors
        shownTypes: Sensor.TypeZGPSwitch
//                    | Sensor.TypeDaylight
                    | Sensor.TypeZLLSwitch
//                    | Sensor.TypeAll
    }

    ListView {
        model: filterModel
        anchors.fill: parent
        delegate: ListItem {
            height: units.gu(10)
            RowLayout {
                anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2); topMargin: units.gu(1); bottomMargin: units.gu(1) }
                spacing: units.gu(1)
                Icon {
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    source: {
                        switch(model.type) {
                        case Sensor.TypeZGPSwitch:
                            return "images/tap_outline.svg"
                        case Sensor.TypeDaylight:
                            return "images/bridge_outline.svg"
                        case Sensor.TypeZLLSwitch:
                            return "images/dimmer_outline.svg"
                        }
                        return "";
                    }
                }

                Label {
                    text: model.name
                    Layout.fillWidth: true
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("SensorPage.qml"), {sensors: root.sensors, sensor: filterModel.get(index), rules: root.rules, lights: root.lights, groups: root.groups, scenes: root.scenes });
        }
    }

    Label {
        width: parent.width - units.gu(8)
        anchors.centerIn: parent
        fontSize: "large"
        text: "No Hue Tap or Hue Dimmer device connected to the bridge. You can add new remotes in the \"Bridge Control\" section."
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        visible: filterModel.count === 0 && !root.busy
    }
}
