import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.Pickers 1.3
import Ubuntu.Components.ListItems 1.3

Column {
    id: root
    spacing: units.gu(1)

    property var date: new Date()
    property alias recurring: recurringSwitch.checked
    property string weekdays: "01111100"

    property int margins: 0

    function showPastTimeError() {
        PopupUtils.open(pastTimeError)
    }

    ListItem {
        highlightColor: color
        RowLayout {
            anchors { verticalCenter: parent.verticalCenter; left: parent.left; right: parent.right; leftMargin: root.margins; rightMargin: root.margins }
            Label {
                Layout.fillWidth: true
                text: "Recurring alarm"
            }
            Switch {
                id: recurringSwitch
            }
        }
    }

    ListItem {
        RowLayout {
            anchors.fill: parent
            anchors { leftMargin: root.margins; rightMargin: root.margins }
            Label {
                Layout.fillWidth: true
                text: root.recurring ? i18n.tr("Time") : i18n.tr("Date & Time")
            }
            Label {
                text: "" + (root.recurring ? "" : Qt.formatDate(root.date)) + " " + Qt.formatTime(root.date)
            }
        }

        onClicked: {
            var popup = PopupUtils.open(dateTimePicker, root, {date: root.date})
            popup.closed.connect(function(date) {
                root.date = date
            })
        }
        Component {
            id: dateTimePicker
            Dialog {
                id: dtp
                title: root.recurring ? i18n.tr("Time") : i18n.tr("Date & Time")
                signal closed(var date)

                property var date: new Date()

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
    }

    ListItem {
        id: recurrenceSelector

        states: [
            State {
                name: "invisible"; when: !root.recurring
                PropertyChanges { target: recurrenceSelector; height: 0; opacity: 0 }
            }
        ]
        transitions: [
            Transition {
                from: "*"
                to: "*"
                UbuntuNumberAnimation { properties: "height,opacity" }
            }
        ]
        ColumnLayout {
            anchors.verticalCenter: parent.verticalCenter
            anchors { left: parent.left; right: parent.right; leftMargin: root.margins; rightMargin: root.margins }
            Label {
                text: "Recurrence"
            }
            Label {
                text: {
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

            }
        }

        onClicked: {
            var popup = PopupUtils.open(recurrancePicker, root)
            popup.closed.connect(function(weekdays) {
                recurrenceSelector.weekdays = weekdays;
            })
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
    }

    Component {
        id: pastTimeError
        Dialog {
            id: dialog
            title: "Invalid Time"
            text: "The selected time is in the past. Please select a time in the future."
            Button {
                color: UbuntuColors.orange
                text: "OK"
                onClicked: PopupUtils.close(dialog)
            }
        }
    }
}

