import QtQuick 2.4
import Ubuntu.Components 1.3
import QtQuick.Layouts 1.1
import Hue 0.1
Page {
    title: "Bridge info"
    GridLayout {
        anchors.fill: parent
        anchors.margins: units.gu(1)
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
        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }


    }
}
