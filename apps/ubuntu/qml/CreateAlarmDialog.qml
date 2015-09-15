import QtQuick 2.3
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Pickers 1.3
import Hue 0.1

Dialog {
    id: root
    title: "Create Alarm"
    text: root.light ? "Create an alarm for %1. The current power, brightness and color values will be restored at the given time.".arg(root.light.name)
                     : "Create an alarm for %1. The scene will be activated at the given time.".arg(root.scene.name)

    property var light: null
    property var scene: null
    property var schedules: null
    property var date: new Date()

    TextField {
        id: alarmName
        placeholderText: "Alarm name"
        text: "Alarm on %1".arg(root.light ? root.light.name : root.scene.name)
    }

    AlarmDatePicker {
        id: alarmDateTime
    }

    Button {
        text: "OK"
        enabled: alarmName.text
        color: UbuntuColors.green
        onClicked: {
            if (alarmDateTime.recurring) {
                if (root.scene) {
                    root.schedules.createRecurringAlarmForScene(alarmName.text, root.scene.id, alarmDateTime.date, alarmDateTime.weekdays)
                } else if (root.light.isGroup) {
                    root.schedules.createRecurringAlarmForGroup(alarmName.text, root.light.id, root.light.on, light.bri, light.color, alarmDateTime.date, alarmDateTime.weekdays)
                } else {
                    root.schedules.createRecurringAlarmForLight(alarmName.text, root.light.id, root.light.on, light.bri, light.color, alarmDateTime.date, alarmDateTime.weekdays)
                }
            } else {
                if (alarmDateTime.date < new Date()) {
                    alarmDateTime.showPastTimeError()
                    return;
                }
                if (root.scene) {
                    root.schedules.createSingleAlarmForScene(alarmName.text, root.scene.id, alarmDateTime.date);
                } else if (root.light.isGroup) {
                    root.schedules.createSingleAlarmForGroup(alarmName.text, root.light.id, root.light.on, light.bri, light.color, alarmDateTime.date)
                } else {
                    root.schedules.createSingleAlarmForLight(alarmName.text, root.light.id, root.light.on, light.bri, light.color, alarmDateTime.date)
                }
            }
            PopupUtils.close(root)
        }
    }
    Button {
        text: "Cancel"
        color: UbuntuColors.red
        onClicked: {
            PopupUtils.close(root)
        }
    }
}

