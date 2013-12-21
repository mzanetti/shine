import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1
import Ubuntu.Components.Popups 0.1
import Hue 0.1

MainView {
    width: units.gu(50)
    height: units.gu(75)

    Tabs {
        anchors.fill: parent

        Tab {
            title: "Lights"
            LightsPage {

            }
        }

        Tab {
            title: "Groups"
            GroupsPage {

            }
        }
    }
}
