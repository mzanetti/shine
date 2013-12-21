import QtQuick 1.1
import com.nokia.meego 1.0
import Hue 0.1

PageStackWindow {
    id: appWindow

    Component.onCompleted: {
        theme.inverted = true

        if (!hueBridge.username) {
            var component = Qt.createComponent(Qt.resolvedUrl("ConnectionDialog.qml"));
            var popup = component.createObject(mainPage);
            popup.open();
        }
    }

    initialPage: mainPage

    MainPage {
        id: mainPage
    }

    ToolBarLayout {
        id: commonTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTr("Sample menu item") }
        }
    }
}
