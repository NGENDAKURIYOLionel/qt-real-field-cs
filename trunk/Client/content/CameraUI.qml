import Qt 4.7
import QtMultimediaKit 1.1

Item {
    id : cameraUI
    property int w
    property int h

    state: "PhotoCapture"

    states: [
        State {
            name: "PhotoCapture"
            StateChangeScript {
                script: {
                    camera.visible = true
                    camera.focus = true
                    stillControls.visible = true
                    photoPreview.visible = false
                    //photoLoginPhotoFrame.visible = true
                }
            }
        },
        State {
            name: "PhotoPreview"
            StateChangeScript {
                script: {
                    camera.visible = false
                    stillControls.visible = false
                    photoPreview.visible = true
                    photoPreview.focus = true
                    //photoLoginPhotoFrame.visible = false
                }
            }
        }
    ]

    PhotoPreview {
        id : photoPreview
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width : cameraUI.w
        height : cameraUI.h
        onClosed: cameraUI.state = "PhotoCapture"
        focus: visible

        Keys.onPressed : {
            //return to capture mode if the shutter button is touched
            if (event.key == Qt.Key_CameraFocus) {
                cameraUI.state = "PhotoCapture"
                event.accepted = true;
            }
        }
    }

    Camera {
        id: camera
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width : cameraUI.w
        height : cameraUI.h
        focus : visible //to receive focus and capture key events
        captureResolution : "640x480"

        onImageCaptured : {
            photoPreview.source = preview
            stillControls.previewAvailable = true
            cameraUI.state = "PhotoPreview"
        }
    }

    CaptureControls {
        id: stillControls
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 150
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -75
        camera: camera
        onPreviewSelected: cameraUI.state = "PhotoPreview"
    }

}
