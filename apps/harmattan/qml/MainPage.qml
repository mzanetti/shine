import QtQuick 1.1
import com.nokia.meego 1.0
import QtMobility.systeminfo 1.1
import shine 1.0

Page {

    orientationLock: PageOrientation.LockPortrait

    tools:ToolBarLayout{
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            onClicked: (myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }
    Component {
        id: button
        Rectangle {
            anchors.fill: parent
            border.color: "black"
            border.width: 1
            color: "green"

        }

    }

    Column {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        OnOffButton {
            width: parent.width
            height: parent.height / 5
        }

        MainButton{
            text: "Ambilight"
            onClicked: client.setLightMode(3);
            width: parent.width
            height: parent.height / 5
        }

        MainButton{
            text: "Moodlight"
            onClicked: {
                client.setLightMode(2);
                pageStack.push(Qt.createComponent("DetailsPage.qml"));
            }
            width: parent.width
            height: parent.height / 5
        }

        MainButton{
            text: "Remote"
            onClicked: {
                onClicked: pageStack.push(Qt.createComponent("SystemCommandPage.qml"))
            }
            width: parent.width
            height: parent.height / 5
        }


    }


}
