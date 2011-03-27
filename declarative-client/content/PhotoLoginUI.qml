import QtQuick 1.0

Item {
    state: "invisible"
    visible: false
    width: 300; height: 236
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: -70


    CameraUI {
        anchors.fill: parent
        w: 300
        h: 300
    }

    Image {
        id: photoLoginPhotoFrame
        source: "pics/photoFrame2.png"
        anchors.fill: parent
    }

    Button {
        id: photoLoginButton
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 170
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Login"
        source: "pics/toolbutton.png"
        fontSize: 20
        width: 105
        height: 50
        opacity: 0.65
        onClicked: Qt.quit()
    }

    states: [
        State {
            name: "invisible"
            PropertyChanges { target: photoLoginUI; visible: false }
        },
        State {
            name: "visible"
            PropertyChanges { target: photoLoginUI; visible: true }
        }
    ]
    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }
    function toggle() {
        if (state == "visible") {
            state = "invisible";
            usernameLoginUI.state = "invisible";
        }
        else {
            state = "visible";
            usernameLoginUI.state = "invisible";
        }
    }

}
