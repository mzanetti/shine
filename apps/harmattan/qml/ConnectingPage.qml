// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0

Page{

    Column{
        anchors.centerIn: parent
        spacing: 10
        width: parent.width * 0.75

        TextField{
            id: ipTextField
            text: settings.ipaddress
            width: parent.width

        }

        Button{
            id: connectButton
            text: "Connect"
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            onClicked:{
                settings.ipaddress = ipTextField.text
                client.connectToServer();
            }
        }
    }

}
