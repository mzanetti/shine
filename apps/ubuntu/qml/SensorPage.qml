import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3
import Hue 0.1

Page {
    id: root
    title: sensor.name

    property var sensors: null
    property var sensor: null
    property var rules: null
    property var lights: null
    property var groups: null
    property var scenes: null

    SensorsFilterModel {
        sensors: root.sensors

    }

    head {
        actions: [
            Action {
                iconName: "save"
                onTriggered: {
                    root.saveRules()
                }
            }
        ]
    }

    RulesFilterModel {
        id: existingRulesFilter
        rules: root.rules
    }

    Connections {
        target: sensorLoader.item
        onButtonIdChanged: {
            print("button id changed", sensorLoader.item.buttonId)
            loadRules();
        }
    }

    Component.onCompleted: loadRules();

    function loadRules() {
        // Reset selected scenes
        scenesListView.selectedScenes = []
        scenesListView.model = null;
        var recipes = [];

        lightsAndGroupsList.currentIndex = -1;

        // Find all the rules for this button
        existingRulesFilter.conditionFilter = sensorLoader.item.createFilter();

        // Set the view state according to the rules
        for (var i = 0; i < existingRulesFilter.count; i++) {
            var rule = existingRulesFilter.get(i);
            print("have rule", rule.name)
            if (rule.name.indexOf("Shine-L") === 0) {
                lightsOrSceneSelector.selectedIndex = 0
                var lightId = rule.name.substring(8, rule.name.indexOf("-", 8))
                lightsAndGroupsList.selectLight(lightId)
            } else if (rule.name.indexOf("Shine-G") === 0) {
                lightsOrSceneSelector.selectedIndex = 0
                var groupId = rule.name.substring(8, rule.name.indexOf("-", 8))
                lightsAndGroupsList.selectGroup(groupId)
            } else if (rule.name.indexOf("Shine-S") === 0) {
                lightsOrSceneSelector.selectedIndex = 2
                print("have scene:", rule.name.substring(7, rule.name.length))
                scenesListView.selectedScenes.push(rule.name.substring(7, rule.name.length));
            } else if (rule.name.indexOf("Shine-R") === 0) {
                lightsOrSceneSelector.selectedIndex = 1
                if (rule.name.indexOf("Shine-RG") === 0) {
                    lightsAndGroupsList.selectGroup(rule.name.substring(8, rule.name.indexOf("-", 8)))
                } else {
                    lightsAndGroupsList.selectLight(rule.name.substring(8, rule.name.indexOf("-", 8)))
                }
                var name = rule.name.substring(rule.name.indexOf("-", 9) + 1, rule.name.length);
                print("name", name, recipes.indexOf(name))
                if (recipes.indexOf(name) === -1) {
                    recipes.push(name)
                }
            } else if (rule.name.indexOf("Shine-DUL") === 0) {
                lightsOrSceneSelector.selectedIndex = 3;
                var lightId = rule.name.substring(10, rule.name.length)
                print("lightId", lightId)
                lightsAndGroupsList.selectLight(lightId)
            } else if (rule.name.indexOf("Shine-DDL") === 0) {
                lightsOrSceneSelector.selectedIndex = 4;
                var lightId = rule.name.substring(10, rule.name.length)
                print("lightId", lightId)
                lightsAndGroupsList.selectLight(lightId)
            } else if (rule.name.indexOf("Shine-DUG")) {
                lightsOrSceneSelector.selectedIndex = 3;
                var groupId = rule.name.substring(10, rule.name.length)
                lightsAndGroupsList.selectGroup(groupId)
            } else if (rule.name.indexOf("Shine-DDG")) {
                lightsOrSceneSelector.selectedIndex = 4;
                var groupId = rule.name.substring(10, rule.name.length)
                lightsAndGroupsList.selectGroup(groupId)
            }
        }
        scenesListView.model = scenesFilterModel;
        recipeSelector.recipes = recipes;
    }

    function saveRules() {
        busyTimer.start();
        var buttonId = sensorLoader.item.buttonId;

        // Deleting old rules
        existingRulesFilter.conditionFilter = sensorLoader.item.createFilter();
        for (var i = 0; i < existingRulesFilter.count; i++) {
            print("should delete rule", existingRulesFilter.get(i).id, existingRulesFilter.get(i).name)
            root.rules.deleteRule(existingRulesFilter.get(i).id)
        }

        var uniqueId = root.sensor.uniqueId || "000000"
        print("uniqueid is", root.sensor.uniqueId, root.sensor)
        var helperSensor = root.sensors.findOrCreateHelperSensor("HueTapHelper" + buttonId, uniqueId);
        if (!helperSensor) {
            print("No helper sensor found. bailing out...")
            return;
        }
        

        var conditions = []
        var actions = []

        print("current selection", lightsOrSceneSelector.selectedIndex)
        if (lightsOrSceneSelector.selectedIndex == 0) {
            print("Creating light toggle rules")
            var lightOrGroup = lightsAndGroupsList.model.get(lightsAndGroupsList.currentIndex);
            var ruleName = "Shine-"

            conditions = sensorLoader.item.createSensorConditions();
            conditions.push(rules.createHelperCondition(helperSensor.id, "eq", "0"))

            if (lightOrGroup.type === "light") {
                actions.push(root.rules.createLightAction(lightOrGroup.id, true))
                ruleName += "L"
            } else {
                actions.push(root.rules.createGroupAction(lightOrGroup.id, true))
                ruleName += "G"
            }
            ruleName += "-" + lightOrGroup.id + "-"

            actions.push(root.rules.createHelperAction(helperSensor.id, 1))
            rules.createRule(ruleName + "on", conditions, actions);

            conditions = sensorLoader.item.createSensorConditions();
            conditions.push(root.rules.createHelperCondition(helperSensor.id, "gt", "0"))
            actions = []
            if (lightOrGroup.type === "light") {
                actions.push(root.rules.createLightAction(lightOrGroup.id, false))
            } else {
                actions.push(root.rules.createGroupAction(lightOrGroup.id, false))
            }

            actions.push(root.rules.createHelperAction(helperSensor.id, 0))
            rules.createRule(ruleName + "off", conditions, actions)

        } else if (lightsOrSceneSelector.selectedIndex == 2) {
            print("Creating activate scenes rules")
            print("scenelength", scenesListView.selectedScenes.length)
            for (var i = 0; i < scenesListView.selectedScenes.length; i++) {
                print("should add scene", scenesListView.selectedScenes[i])
                var scene = scenesListView.selectedScenes[i]
                var ruleName = "Shine-S" + scene;
                var conditions = sensorLoader.item.createSensorConditions();
                if (i == 0 && scenesListView.selectedScenes.length > 1) {
                    conditions.push(rules.createHelperCondition(helperSensor.id, "lt", "" + buttonId + "1"))
                } else if (scenesListView.selectedScenes.length > 1) {
                    conditions.push(rules.createHelperCondition(helperSensor.id, "eq", "" + buttonId + i))
                }

                actions = [];
                actions.push(root.rules.createSceneAction(scenesListView.selectedScenes[i]));
                if (i == scenesListView.selectedScenes.length - 1 && scenesListView.selectedScenes.length > 1) {
                    actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + "0"))
                } else if (scenesListView.selectedScenes.length > 1) {
                    actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + (i + 1)))
                }

                rules.createRule(ruleName, conditions, actions);
            }
            if (scenesListView.selectedScenes.length > 1) {
                var conditions = sensorLoader.item.createSensorConditions()
                conditions.push(rules.createHelperCondition(helperSensor.id, "gt", "" + buttonId + (scenesListView.selectedScenes.length - 1)))

                actions = [];
                actions.push(root.rules.createSceneAction(scenesListView.selectedScenes[0]));
                actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + "1"))
                rules.createRule("Shine-S" + scenesListView.selectedScenes[0], conditions, actions);
            }
        } else if (lightsOrSceneSelector.selectedIndex == 1) {  // recipies
            print("Creating recipe rules")
            var lightOrGroup = lightsAndGroupsList.model.get(lightsAndGroupsList.currentIndex);

            for (var i = 0; i < recipeSelector.recipes.length; i++) {
                var recipe = { name: "off"};
                if (recipeSelector.recipes[i] !== "off") {
                    recipe = lightRecipeModel.getByName(recipeSelector.recipes[i]);
                }
                print("should add recipe", recipeSelector.recipes[i], recipe.name)
                var ruleName = "Shine-R"

                var conditions = sensorLoader.item.createSensorConditions();
                if (i == 0 && recipeSelector.recipes.length > 1) {
                    conditions.push(rules.createHelperCondition(helperSensor.id, "lt", "" + buttonId + "1"))
                } else if (recipeSelector.recipes.length > 1) {
                    conditions.push(rules.createHelperCondition(helperSensor.id, "eq", "" + buttonId + i))
                }

                actions = [];

                print("***saving rule", lightOrGroup.isGroup, lightOrGroup.name, lightsAndGroupsList.model)
                if (lightOrGroup.isGroup) {
                    ruleName += "G";
                    if (recipe.name === "off") {
                        actions.push(root.rules.createGroupAction(lightOrGroup.id, false))
                    } else {
                        actions.push(root.rules.createGroupColorAction(lightOrGroup.id, recipe.color, recipe.bri));
                    }
                } else {
                    ruleName += "L";
                    if (recipe.name === "off") {
                        actions.push(root.rules.createLightAction(lightOrGroup.id, false))
                    } else {
                        actions.push(root.rules.createLightColorAction(lightOrGroup.id, recipe.color, recipe.bri));
                    }
                }
                ruleName += lightOrGroup.id + "-" + recipe.name;

                if (i == recipeSelector.recipes.length - 1 && recipeSelector.recipes.length > 1) {
                    actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + "0"))
                } else if (recipeSelector.recipes.length > 1) {
                    actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + (i + 1)))
                }

                rules.createRule(ruleName, conditions, actions);
            }
            if (recipeSelector.recipes.length > 1) {
                var recipe = { name: "off" };
                if (recipeSelector.recipes[0] !== "off") {
                    recipe = lightRecipeModel.getByName(recipeSelector.recipes[0])
                }
                var conditions = sensorLoader.item.createSensorConditions()
                conditions.push(rules.createHelperCondition(helperSensor.id, "gt", "" + buttonId + (recipeSelector.recipes.length - 1)))
                var ruleName = "Shine-R"
                actions = [];
                if (lightOrGroup.isGroup) {
                    ruleName += "G";
                    if (recipe.name === "off") {
                        actions.push(root.rules.createGroupAction(lightOrGroup.id, false));
                    } else {
                        actions.push(root.rules.createGroupColorAction(lightOrGroup.id, recipe.color, recipe.bri));
                    }
                } else {
                    ruleName += "L";
                    if (recipe.name === "off") {
                        actions.push(root.rules.createLightAction(lightOrGroupId, false));
                    } else {
                        actions.push(root.rules.createLightColorAction(lightOrGroup.id, recipe.color, recipe.bri));
                    }
                }
                ruleName += lightOrGroup.id + "-" + recipe.name;
                actions.push(rules.createHelperAction(helperSensor.id, "" + buttonId + "1"))
                rules.createRule(ruleName, conditions, actions);
            }
        } else if (lightsOrSceneSelector.selectedIndex == 5) { // Sleep timer
            var lightOrGroup = lightsAndGroupsList.model.get(lightsAndGroupsList.currentIndex);
            var ruleName = "Shine-T-"
            conditions = sensorLoader.item.createSensorConditions();
            if (lightOrGroup.type === "light") {
                actions.push(root.rules.createLightTimerActions(lightOrGroup))
            } else {
                actions.push(root.rules.createGroupTimerActions(lightOrGroup))
            }
            rules.createRule(ruleName, conditions, actions);

        }else { // Dimmer
            print("Creating dimmer rules")
            var lightOrGroup = lightsAndGroupsList.model.get(lightsAndGroupsList.currentIndex);

            var ruleName = "Shine-D"

            conditions = sensorLoader.item.createSensorConditions();

            var dimAction;
            if (lightsOrSceneSelector.selectedIndex == 3) {
                dimAction = Rules.DimActionUp;
                ruleName += "U";
            } else {
                dimAction = Rules.DimActionDown;
                ruleName += "D";
            }
            if (lightOrGroup.type === "light") {
                actions.push(root.rules.createLightDimmerAction(lightOrGroup.id, dimAction))
                ruleName += "L"
            } else {
                actions.push(root.rules.createGroupDimmerAction(lightOrGroup.id, dimAction))
                ruleName += "G"
            }
            ruleName += "-" + lightOrGroup.id

            actions.push(root.rules.createHelperAction(helperSensor.id, 1))
            rules.createRule(ruleName, conditions, actions);
        }

        rules.refresh();
    }

    ColumnLayout {
        anchors.fill: parent
        id: actionSelection
        spacing: units.gu(1)

        Loader {
            id: sensorLoader
            Layout.fillWidth: true
            Layout.preferredHeight: width / 2
            //            Layout.fillHeight: true
            sourceComponent: {
                switch(root.sensor.type) {
                case Sensor.TypeDaylight:
                    return hueBridgeComponent;
                case Sensor.TypeZGPSwitch:
                    return hueTapComponent;
                case Sensor.TypeZLLSwitch:
                    return hueDimmerComponent;
                }
                return null;
            }
        }

        ItemSelector {
            id: lightsOrSceneSelector
//            model: ["Toggle lights", "Switch lights", "Activate scenes", "Dim Up", "Dim Down", "Set sleep timer"]
            model: ["Toggle lights", "Switch lights", "Activate scenes", "Dim Up", "Dim Down"]
        }
        Item {
            id: recipeSelector
            Layout.fillWidth: true
            Layout.preferredHeight: (width - units.gu(6)) / 4
            visible: lightsOrSceneSelector.selectedIndex == 1
            opacity: lightsOrSceneSelector.currentlyExpanded ? 0 : 1
            Behavior on opacity { UbuntuNumberAnimation {} }

            property var recipes: []
            function toggleRecipe(name) {
                print("toggling", name)
                var removed = false;
                var list = [];
                for (var i = 0; i < recipeSelector.recipes.length; i++) {
                    if (recipeSelector.recipes[i] === name) {
                        print("removing", name)
                        removed = true;
                    } else {
                        print("adding", recipeSelector.recipes[i])
                        list.push(recipeSelector.recipes[i]);
                    }
                }
                if (!removed) {
                    list.push(name)
                }
                recipeSelector.recipes = list;
            }

            RowLayout {
                anchors.fill: parent; anchors.margins: units.gu(1)
                spacing: units.gu(2)
                Repeater {
                    model: LightRecipeModel {
                        id: lightRecipeModel
                    }
                    ColorButton {
                        Layout.preferredHeight: width
                        Layout.fillWidth: true
                        source: "images/" + model.name + ".svg"
                        onClicked: recipeSelector.toggleRecipe(model.name)

                        Icon {
                            anchors { right: parent.right; bottom: parent.bottom }
                            height: parent.height / 2
                            width: height
                            name: "tick"
                            color: UbuntuColors.green

                            visible: {
                                for (var i = 0; i < recipeSelector.recipes.length; i++) {
                                    if (recipeSelector.recipes[i] === model.name) {
                                        return true;
                                    }
                                }
                                return false;
                            }
                        }
                    }
                }
                ColorButton {
                    Layout.preferredHeight: width
                    Layout.fillWidth: true
                    source: "image://theme/torch-off"
                    onClicked: recipeSelector.toggleRecipe("off")

                    Icon {
                        anchors { right: parent.right; bottom: parent.bottom }
                        height: parent.height / 2
                        width: height
                        name: "tick"
                        color: UbuntuColors.green

                        visible: {
                            for (var i = 0; i < recipeSelector.recipes.length; i++) {
                                if (recipeSelector.recipes[i] === "off") {
                                    return true;
                                }
                            }
                            return false;
                        }
                    }
                }
            }
        }

        LightsAndGroupsList {
            id: lightsAndGroupsList
            lights: root.lights
            groups: root.groups
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: lightsOrSceneSelector.selectedIndex !== 2
            opacity: lightsOrSceneSelector.currentlyExpanded ? 0 : 1
            Behavior on opacity { UbuntuNumberAnimation {} }
        }

        ListView {
            id: scenesListView
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: 0
            clip: true
            visible: lightsOrSceneSelector.selectedIndex == 2
            opacity: lightsOrSceneSelector.currentlyExpanded ? 0 : 1
            Behavior on opacity { UbuntuNumberAnimation {} }

            property var selectedScenes: []
            model: scenesFilterModel
            ScenesFilterModel {
                id: scenesFilterModel
                scenes: root.scenes
                hideOtherApps: true
            }
            delegate: ListItem {
                height: units.gu(5)
                RowLayout {
                    anchors { fill: parent; leftMargin: units.gu(2); rightMargin: units.gu(2); topMargin: units.gu(1); bottomMargin: units.gu(1) }
                    Label {
                        Layout.fillWidth: true
                        text: model.name
                    }
                    Icon {
                        id: useSceneCheckBox
                        Layout.fillHeight: true
                        Layout.preferredWidth: height
                        name: "tick"
                        visible: {
                            print("evaluating visible for", model.name, scenesListView.selectedScenes.length)
                            for (var i = 0; i < scenesListView.selectedScenes.length; i++) {
                                if (scenesListView.selectedScenes[i] == model.id) {
                                    print("scene is checked", model.name)
                                    return true;
                                }
                            }
                            return false;
                        }
                    }
                }

                onClicked: {
                    var removed = false;
                    var list = [];
                    for (var i = 0; i < scenesListView.selectedScenes.length; i++) {
                        if (scenesListView.selectedScenes[i] === model.id) {
                            removed = true;
                        } else {
                            list.push(scenesListView.selectedScenes[i])
                        }
                    }
                    if (!removed){
                        list.push(model.id)
                    }
                    scenesListView.selectedScenes = list;
                }
            }
        }

    }

    MouseArea {
        anchors.fill: parent
        visible: busyRect.opacity > 0
        Timer {
            id: busyTimer
            interval: 5000
            repeat: false
        }
        Rectangle {
            id: busyRect
            anchors.fill: parent
            color: "black"
            opacity: busyTimer.running ? 0.7 : 0
            Behavior on opacity { UbuntuNumberAnimation {} }
        }
        Column {
            width: parent.width
            anchors.centerIn: parent
            spacing: units.gu(5)
            ActivityIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: parent.visible
            }
            Label {
                width: parent.width - units.gu(8)
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                text: "Please wait while the configuration is saved to the bridge..."
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                fontSize: "large"
            }

        }

    }

    Component {
        id: hueBridgeComponent

        Item {
            id: hueBridge

            property int buttonId: 0

            property bool day: true

            function createSensorConditions() {
                return root.rules.createDaylightConditions(root.sensor.id, hueBridge.day);
            }

            function createFilter() {
                var filter = new Object();
                filter["address"] = "/sensors/" + root.sensor.id + "/state/daylight"
                filter["operator"] = "eq"
                filter["value"] = hueBridge.day
                return filter
            }

            Image {
                source: "images/bridge_" + (hueBridge.day ? "outline" : "filled") + ".svg"
                height: parent.height
                width: height
                sourceSize.height: height
                sourceSize.width: width
                anchors.horizontalCenter: parent.horizontalCenter

                MouseArea {
                    anchors.fill: parent
                    onClicked: hueBridge.day = !hueBridge.day
                }
            }
        }
    }
    Component {
        id: hueTapComponent
        Item {
            id: hueTap

            property int buttonId: 34

            function createSensorConditions() {
                return root.rules.createHueTapConditions(root.sensor.id, hueTap.buttonId);
            }

            function createFilter() {
                var filter = new Object();
                filter["address"] = "/sensors/" + root.sensor.id + "/state/buttonevent"
                filter["operator"] = "eq"
                filter["value"] = hueTap.buttonId
                return filter
            }

            Image {
                source: "images/tap_outline_" +  hueTap.buttonId + ".svg"
                height: parent.height
                width: height
                sourceSize.height: height
                sourceSize.width: width
                anchors.horizontalCenter: parent.horizontalCenter
                MouseArea {
                    anchors.fill: parent
                    onClicked: hueTap.buttonId = 34
                }

                MouseArea {
                    height: parent.height / 4
                    width: height
                    anchors { left: parent.left; leftMargin: width / 2; verticalCenter: parent.verticalCenter }
                    onClicked: hueTap.buttonId = 16
                }
                MouseArea {
                    height: parent.height / 4
                    width: height
                    anchors { bottom: parent.bottom; bottomMargin: width / 2; horizontalCenter: parent.horizontalCenter }
                    onClicked: hueTap.buttonId = 17
                }
                MouseArea {
                    height: parent.height / 4
                    width: height
                    anchors { right: parent.right; rightMargin: width / 2; verticalCenter: parent.verticalCenter }
                    onClicked: hueTap.buttonId = 18
                }
            }
        }
    }

    Component {
        id: hueDimmerComponent
        Item {
            id: hueDimmer
            property int buttonId: buttonNumber + "00" + buttonMode

            property int buttonNumber: 1
            property int buttonMode: pressCheckBox.checked ? 2 : 1

            function createSensorConditions() {
                return root.rules.createHueDimmerConditions(root.sensor.id, hueDimmer.buttonId);
            }

            function createFilter() {
                var filter = new Object();
                filter["address"] = "/sensors/" + root.sensor.id + "/state/buttonevent"
                filter["operator"] = "eq"
                filter["value"] = hueDimmer.buttonId
                return filter
            }

            Row {
                anchors.fill: parent
                Image {
                    source: "images/dimmer_outline_" + hueDimmer.buttonNumber + ".svg"
                    height: parent.height
                    width: height
                    sourceSize.height: height
                    sourceSize.width: width
//                    anchors.horizontalCenter: parent.horizontalCenter

                    MouseArea {
                        height: parent.height / 3
                        width: parent.width / 3
                        anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
                        onClicked: hueDimmer.buttonNumber = 1
                    }
                    MouseArea {
                        height: parent.height / 6
                        width: parent.width / 3
                        anchors { centerIn: parent; verticalCenterOffset: -height / 2 }
                        onClicked: hueDimmer.buttonNumber = 2
                    }
                    MouseArea {
                        height: parent.height / 6
                        width: parent.width / 3
                        anchors { centerIn: parent; verticalCenterOffset: height / 2 }
                        onClicked: hueDimmer.buttonNumber = 3
                    }
                    MouseArea {
                        height: parent.height / 3
                        width: parent.width / 3
                        anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter }
                        onClicked: hueDimmer.buttonNumber = 4
                    }
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: units.gu(1)
                    Row {
                        spacing: units.gu(1)
                        CheckBox {
                            id: pressCheckBox
                            checked: true
                            onClicked: {
                                holdCheckBox.checked = false;
                            }
                        }
                        Label {
                            text: "Press"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Row {
                        spacing: units.gu(1)
                        CheckBox {
                            id: holdCheckBox
                            onClicked: {
                                pressCheckBox.checked = false;
                            }
                        }
                        Label {
                            text: "Hold"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
    }
}
