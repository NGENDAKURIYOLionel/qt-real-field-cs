import QtQuick 1.0
import QtMultimediaKit 1.1

Item {
    state: "invisible"
    visible: false
    width: 300; height: 236
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    anchors.verticalCenterOffset: -70

    PhotoPreview {
        id : photoPreview
        anchors.fill: parent
        anchors.top: parent.top
        anchors.topMargin: -50
        width : 280
        height : 280
        visible: false
        onClosed: {
            camera.visible = true
            camera.focus = true
            stillControls.visible = true
            photoPreview.visible = false
        }

        focus: visible

        Keys.onPressed : {
            //return to capture mode if the shutter button is touched
            if (event.key == Qt.Key_CameraFocus) {
                camera.visible = true
                camera.focus = true
                stillControls.visible = true
                photoPreview.visible = false
                event.accepted = true;
            }
        }
    }

    Camera {
        id: camera
        visible: true
        anchors.fill: parent
        anchors.top: parent.top
        anchors.topMargin: -50
        width : 280
        height : 280
        focus : visible //to receive focus and capture key events
        captureResolution : "640x480"

        onImageCaptured : {
            photoPreview.source = preview
            stillControls.previewAvailable = true
            camera.visible = false
            stillControls.visible = false
            photoPreview.visible = true
            photoPreview.focus = true
        }
    }

    CaptureControls {
        id: stillControls
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 150
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -75
        visible: true
        camera: camera
        onPreviewSelected: {
            camera.visible = false
            stillControls.visible = false
            photoPreview.visible = true
            photoPreview.focus = true
        }
    }

    Rectangle { radius: 10.0; opacity:  0.75
                width: 250; height: 48
                anchors.top: parent.top
                anchors.topMargin: 210
                anchors.horizontalCenter: parent.horizontalCenter
               LineEdit {
                   id: userNameLineEdit
                   fontSize: 22
                   text: "    Your Email"
                   echoMode: TextInput.Normal
               }
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
        onClicked: {
//            camera.captureResolution = Qt.size(800,640)
//            camera.visible = false
//            camera.enabled = false
//            camera.stop
            client.sendImage(userNameLineEdit.text)
        }

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
        NumberAnimation { properties: "opacity"; duration: 250 }
    }

    function toggle() {
        if (state == "visible") {
            state = "invisible";
            usernameLoginUI.state = "invisible";
            loginFail.state = "invisible"
        }
        else {
            state = "visible";
            usernameLoginUI.state = "invisible";
            loginFail.state = "invisible"
        }
    }

}
