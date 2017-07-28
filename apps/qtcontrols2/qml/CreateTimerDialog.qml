import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Pickers 1.3
import Hue 0.1

Dialog {
    id: root
    title: "Set timer"
    text: "%1 will be switched off in".arg(root.light.name)

    property var schedules: null
    property var light: null

    DatePicker {
        id: dateTime
        mode: "Hours|Minutes"
        Component.onCompleted: {
            var newDate = new Date(30 * 60 * 1000);
            newDate.setHours(0); // For some reason the above will create 1:30 instead of 0:30
            date = newDate;
        }
    }
    Button {
        text: "OK"
        color: UbuntuColors.green
        onClicked: {
            if (root.light.isGroup) {
                root.schedules.createTimerForGroup("%1 off".arg(root.light.name), root.light.id, false, 0, "white", dateTime.date)
            } else {
                root.schedules.createTimerForLight("%1 off".arg(root.light.name), root.light.id, false, 0, "white", dateTime.date)
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
