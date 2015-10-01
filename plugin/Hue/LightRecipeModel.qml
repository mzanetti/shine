import QtQuick 2.4

ListModel {

    function getByName(name) {
        for (var i = 0; i < count; i++) {
            if (get(i).name === name) {
                return get(i);
            }
        }
        return null;
    }

    ListElement {
        name: "relax"
        color: "#d5ff2b"
        bri: 144
    }
    ListElement {
        name: "reading"
        color: "#cfff86"
        bri: 240
    }
    ListElement {
        name: "concentrate"
        color: "#d3fcff"
        bri: 220
    }
    ListElement {
        name: "activate"
        color: "#17ddff"
        bri: 200
    }
}
