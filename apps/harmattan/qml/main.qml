import QtQuick 1.1
import com.nokia.meego 1.0
import Hue 0.1

PageStackWindow {
    id: appWindow

    Component.onCompleted: {
        theme.inverted = true

        if (hueBridge.discoveryError) {
            print("TODO: handle discovery error")
        } else if (hueBridge.bridgeFound && !hueBridge.connectedBridge){
            var component = Qt.createComponent(Qt.resolvedUrl("ConnectionDialog.qml"));
            var popup = component.createObject(mainPage);
            popup.open();
        }
    }

    Connections {
        target: hueBridge
        onBridgeFoundChanged: {
            if (!hueBridge.connectedBridge) {
                var component = Qt.createComponent(Qt.resolvedUrl("ConnectionDialog.qml"));
                var popup = component.createObject(mainPage);
                popup.open();
            }
        }
        onDiscoveryErrorChanged: {
            if (hueBridge.discoveryError) {
                PopupUtils.open(errorComponent, root)
            }
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
