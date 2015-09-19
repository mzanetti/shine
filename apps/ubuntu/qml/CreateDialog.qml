import QtQuick 2.3
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3

Dialog {
    id: root
    title: mode == "group" ? "Add group" : "Add scene"
    text: mode == "group" ? "Please enter a name for the new group:" : "Please enter a name for the scene:"

    property string mode: "group" // or "scene"
    property var lights: null
    property var checkedLights: null
    property alias name: nameTextField.text

    signal accepted(string name, var ligths);
    signal rejected();

    Component.onCompleted: {
        print("completed", root.checkedLights)
        if (root.checkedLights) {
            for (var i = 0; i < root.checkedLights.length; i++) {
                print("checked light:", root.checkedLights[i])
                lightsCheckboxes.itemAt(root.checkedLights[i] - 1).checked = true
            }
        }
    }

    TextField {
        id: nameTextField
        width: parent.width - x
    }
    ThinDivider {}

    Repeater {
        id: lightsCheckboxes
        model: root.lights
        delegate: Row {
            width: parent.width
            spacing: units.gu(1)
            property alias checked: checkBox.checked
            CheckBox {
                id: checkBox
                checked: false
            }
            Label {
                text: name
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
    Button {
        text: "OK"
        color: UbuntuColors.green
        enabled: nameTextField.text || nameTextField.inputMethodComposing
        onClicked: {
            nameTextField.focus = false;
            var lightsList = new Array;
            for (var i = 0; i < lightsCheckboxes.count; ++i) {
                if (lightsCheckboxes.itemAt(i).checked) {
                    lightsList.push(lights.get(i).id);
                }
            }

            if (lightsList.length == 0) {
                PopupUtils.open(errorDialog, root)
                return;
            }

            root.accepted(nameTextField.text, lightsList)
            PopupUtils.close(root)
        }
    }
    Button {
        text: "Cancel"
        color: UbuntuColors.red
        onClicked: {
            root.rejected();
            PopupUtils.close(root)
        }
    }

    Component {
        id: errorDialog
        Dialog {
            id: ed
            title: "Error"
            text: "Please select at least one light to be part of the group."
            Button {
                text: "OK"
                onClicked: PopupUtils.close(ed)
            }
        }
    }
}
