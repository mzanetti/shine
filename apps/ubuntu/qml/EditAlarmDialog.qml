import QtQuick 2.3
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Pickers 1.3

Page {
    id: root
    title: "Create Alarm"

    property var lights: null
    property var scenes: null
    property var schedules: null

    readonly property bool recurring: recurringSelector.selectedIndex == 1
    readonly property string mode: modeSelector.selectedIndex == 0 ? "lights" : "scene"

    head {
        actions: [
            Action {
                iconName: "tick"
                enabled: nameTextField.text
                onTriggered: {
                    if (root.recurring) {
                        if (root.mode == "scene") {

                            root.schedules.createRecurringAlarmForScene(nameTextField.text, scenes.get(sceneSelector.selectedIndex).id, dateTime.date, recurrenceSelector.weekdays)
                        }
                    } else {
                        if (root.mode == "scene") {
                            root.schedules.createSingleAlarmForScene(nameTextField.text, scenes.get(sceneSelector.selectedIndex).id, dateTime.date)
                        }
                    }
                    pageStack.pop()
                }
            }
        ]
    }

    Column {
        anchors {
            fill: parent
            topMargin: units.gu(1)
        }
//        anchors.margins: units.gu(1)
        spacing: units.gu(1)

        RowLayout {
            anchors { left: parent.left; right: parent.right; margins: units.gu(2) }
            spacing: units.gu(1)

            Label {
                text: i18n.tr("Name")
            }
            TextField {
                id: nameTextField
                Layout.fillWidth: true
            }
        }

        Column {
            width: parent.width
            ItemSelector {
                id: recurringSelector
                width: parent.width
                model: ["Single alarm", "Recurring alarm"]
            }

            Subtitled {
                id: dateTime
                text: root.recurring ? i18n.tr("Time") : i18n.tr("Date & Time")
                subText: root.recurring ? date.toString("hh:mm") : date.toString(i18n.tr("YYYY-MM-dd hh:mm"))
                property var date: new Date()
                onClicked: {
                    var popup = PopupUtils.open(dateTimePicker, root, {date: date})
                    popup.closed.connect(function(date) {
                        dateTime.date = date
                    })
                }
            }

            Component {
                id: dateTimePicker
                Dialog {
                    id: dtp
                    title: root.recurring ? i18n.tr("Time") : i18n.tr("Date & Time")
                    signal closed(var date)

                    property var date: new Date()

                    Component.onCompleted: {
//                        timePicker.date.setHours(hour)
//                        timePicker.date.setMinutes(minute)
//                        print("**************", timePicker.hours, hour)
                    }

                    DatePicker {
                        id: datePicker
                        date: dtp.date
                        visible: !root.recurring
                    }

                    DatePicker {
                        id: timePicker
                        mode: "Hours|Minutes"
                        date: dtp.date
                    }

                    Button {
                        text: i18n.tr("OK")
                        onClicked: {
                            var mixedDate = datePicker.date;
                            mixedDate.setHours(timePicker.date.getHours())
                            mixedDate.setMinutes(timePicker.date.getMinutes())
                            dtp.closed(mixedDate)
                            PopupUtils.close(dtp)
                        }
                    }
                }
            }

            Subtitled {
                id: recurrenceSelector
                text: "Recurrence"
                subText: {
                    var strings = new Array()
                    if (weekdays[1] == "1") strings.push("Mon")
                    if (weekdays[2] == "1") strings.push("Tue")
                    if (weekdays[3] == "1") strings.push("Wed")
                    if (weekdays[4] == "1") strings.push("Thu")
                    if (weekdays[5] == "1") strings.push("Fri")
                    if (weekdays[6] == "1") strings.push("Sat")
                    if (weekdays[7] == "1") strings.push("Sun")
                    return strings.join(", ")
                }
                visible: root.recurring
                property string weekdays: "00000000"
                onClicked: {
                    var popup = PopupUtils.open(recurrancePicker, root)
                    popup.closed.connect(function(weekdays) {
                        recurrenceSelector.weekdays = weekdays;
                    })
                }
            }
            Component {
                id: recurrancePicker
                Dialog {
                    id: rp
                    title: i18n.tr("Days")
                    signal closed(string weekdays)

                    RowLayout { CheckBox { id: cbMonday; checked: recurrenceSelector.weekdays[1] == "1" } Label { Layout.fillWidth: true; text: "Monday"} }
                    RowLayout { CheckBox { id: cbTuesday; checked: recurrenceSelector.weekdays[2] == "1" } Label { Layout.fillWidth: true; text: "Tuesday"} }
                    RowLayout { CheckBox { id: cbWednesday; checked: recurrenceSelector.weekdays[3] == "1" } Label { Layout.fillWidth: true; text: "Wednesday"} }
                    RowLayout { CheckBox { id: cbThursday; checked: recurrenceSelector.weekdays[4] == "1" } Label { Layout.fillWidth: true; text: "Thursday"} }
                    RowLayout { CheckBox { id: cbFriday; checked: recurrenceSelector.weekdays[5] == "1" } Label { Layout.fillWidth: true; text: "Friday"} }
                    RowLayout { CheckBox { id: cbSaturday; checked: recurrenceSelector.weekdays[6] == "1" } Label { Layout.fillWidth: true; text: "Saturday"} }
                    RowLayout { CheckBox { id: cbSunday; checked: recurrenceSelector.weekdays[7] == "1" } Label { Layout.fillWidth: true; text: "Sunday"} }

                    Button {
                        text: i18n.tr("OK")
                        onClicked: {
                            var weekdays = "0";
                            weekdays += cbMonday.checked ? "1" : "0"
                            weekdays += cbTuesday.checked ? "1" : "0"
                            weekdays += cbWednesday.checked ? "1" : "0"
                            weekdays += cbThursday.checked ? "1" : "0"
                            weekdays += cbFriday.checked ? "1" : "0"
                            weekdays += cbSaturday.checked ? "1" : "0"
                            weekdays += cbSunday.checked ? "1" : "0"
                            rp.closed(weekdays)
                            PopupUtils.close(rp);
                        }
                    }
                }
            }

            ItemSelector {
                id: modeSelector
                width: parent.width
                model: ["Lights", "Scene"]
            }
        }

        Label {
            width: parent.width
            text: root.mode == "lights" ?
                      i18n.tr("Check all the lights that should be controlled by this alarm. The current brightness and color values will be used.")
                    : i18n.tr("Select the scene to be enabled by the alarm")
            wrapMode: Text.WordWrap
        }
        Repeater {
            id: lightsCheckboxes
            model: root.lights
            delegate: Row {
                visible: root.mode == "lights"
                width: parent.width
                spacing: units.gu(1)
                property alias checked: checkBox.checked
                CheckBox {
                    id: checkBox
                    checked: false
                }
                Label {
                    text: name
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        ItemSelector {
            id: sceneSelector
            width: parent.width
            visible: root.mode == "scene"
            model: root.scenes
            delegate: OptionSelectorDelegate {
                text: model.name
            }
        }
    }






//    Button {
//        text: "OK"
//        color: UbuntuColors.green
//        enabled: nameTextField.text.length > 0 || nameTextField.inputMethodComposing
//        onClicked: {
//            var lightsOrScene;
//            if (root.mode == "lights") {
//                var lightsList = new Array;
//                for (var i = 0; i < lightsCheckboxes.count; ++i) {
//                    if (lightsCheckboxes.itemAt(i).checked) {
//                        print("adding light", i)
//                        lightsList.push(lights.get(i).id);
//                        print("list is now", lightsList.length)
//                    }
//                }
//                lightsOrScene = lightsList;
//            } else {
//                lightsOrScene = scenes.get(sceneSelector.selectedIndex).id
//            }
//            root.accepted(nameTextField.text, lightsOrScene)
//            PopupUtils.close(root)
//        }
//    }
//    Button {
//        text: "Cancel"
//        color: UbuntuColors.red
//        onClicked: {
//            root.rejected();
//            PopupUtils.close(root)
//        }
//    }
}
