import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.1
import Hue 0.1

PageStackWindow {
    id: appWindow

    initialPage: Qt.createComponent("ConnectingPage.qml")

    Connections{
        target: client
        onConnectionChanged:{
            print("connection changed",client.isConnected)
            if(client.isConnected){
                pageStack.push(mainPage)
            }else{
                pageStack.pop(null)
            }
        }
    }

    MainPage {
        id: mainPage
    }
}
