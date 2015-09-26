import QtQuick 2.4
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1
import Hue 0.1
Page {
    id: root

    title: "Bridge control"

    property var lights: null

    GridLayout {
        anchors.fill: parent
        anchors.margins: units.gu(2)
        rowSpacing: units.gu(1)
        columns: 2
        SettingsLabel {
            text: "Bridge name:"
        }
        SettingsLabel {
            text: bridgeConfig.name
        }
        SettingsLabel {
            text: "Bridge SW Version:"
        }
        SettingsLabel {
            text: bridgeConfig.swVersion
        }
        SettingsLabel {
            text: "Portal services:"
        }
        SettingsLabel {
            text: bridgeConfig.connectedToPortal ? "Connected" : "Not connected"
        }
        SettingsLabel {
            text: "Update Status:"
        }

        SettingsLabel {
            text: {
                switch (bridgeConfig.updateState) {
                case Configuration.UpdateStateUpToDate:
                    return "Up to date";
                case Configuration.UpdateStateDownloading:
                    return "Downloading update...";
                case Configuration.UpdateStateUpdating:
                    return "Updating...";
                }
                return "";
            }
            Button {
                id: updateButton
                text: "Update now"
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                visible: bridgeConfig.updateState === Configuration.UpdateStateReadyToUpdate
                color: UbuntuColors.green
                enabled: bridgeConfig.connectedToPortal
                onClicked: {
                    bridgeConfig.performUpdate();
                }
            }
        }
        SettingsLabel {
            text: "Update Release Notes:"
            visible: bridgeConfig.updateState == Configuration.UpdateStateReadyToUpdate
        }
        SettingsLabel {
            text: bridgeConfig.swUpdateReleaseNotes
            visible: bridgeConfig.updateState == Configuration.UpdateStateReadyToUpdate
        }

        SettingsLabel {
            id: searchLabel
            Layout.columnSpan: 2
            text: searching ? "The bridge is searching for new devices...\nMake sure the newly installed lights are powered and press any buttons on newly installed Hue Tap switches. New lights should appear in the \"Lights\" section, new Hue Tap switches in the \"Hue Tap\" secion." : ""
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            property bool searching: false
            property var searchTime: new Date(0)

            Timer {
                running: searchLabel.searching
                onTriggered: {
                    if (searchLabel.searchTime < new Date()) {
                        searchLabel.searching = false
                    }
                }
            }

            Button {
                text: "Search for new devices..."
                width: parent.width
                visible: !searchLabel.searching
                color: UbuntuColors.green
                onClicked: {
                    root.lights.searchForNewLights();
                    searchLabel.searchTime = new Date(new Date().getTime() + 60000);
                    searchLabel.searching = true
                }
            }
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }


    }
}
