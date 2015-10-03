/*
 * Copyright 2013-2015 Michael Zanetti
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Michael Zanetti <michael_zanetti@gmx.net>
 */

import QtQuick 2.3
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Ubuntu.Components.Popups 1.3
import Hue 0.1

ShinePage {
    id: root
    title: "Alarms & Timers"
    busy: schedules && schedules.count === 0 && schedules.busy

    property var lights: null
    property var groups: null
    property var schedules: null
    property var scenes: null

    property bool pageActive: false

    Label {
        anchors { left: parent.left; right: parent.right; margins: units.gu(2); verticalCenter: parent.verticalCenter }
        wrapMode: Text.WordWrap
        text: "No alarms or timers set up. You can create alarms and timers in the lights and scenes sections."
        fontSize: "x-large"
        horizontalAlignment: Text.AlignHCenter
        visible: schedules.count === 0 && !root.busy
        z: 2
    }


    ListView {
        anchors.fill: parent
        model: schedules

        delegate: ListItem {
            leadingActions: ListItemActions {
                actions: [
                    Action {
                        iconName: "delete"
                        onTriggered: schedules.deleteSchedule(model.id)
                    }
                ]
            }


            RowLayout {
                anchors {
                    fill: parent
                    leftMargin: units.gu(2)
                    rightMargin: units.gu(2)
                }
                spacing: units.gu(1)
                Icon {
                    Layout.preferredHeight: units.gu(4)
                    implicitWidth: height
                    name: model.type == Schedule.TypeAlarm ? "alarm-clock" : "camera-self-timer"
                }
                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Label {
                        Layout.fillWidth: true
                        text: model.name
                    }
                    Label {
                        Layout.fillWidth: true
                        property string weekdays: model.weekdays
                        property string weekdaysString: {
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

                        text: model.type == Schedule.TypeAlarm ?
                                  (model.recurring ?
                                     Qt.formatTime(model.dateTime) + "  " + weekdaysString
                                   : Qt.formatDateTime(model.dateTime))
                                : Qt.formatTime(model.dateTime)


                    }
                }
            }
        }
    }
}
