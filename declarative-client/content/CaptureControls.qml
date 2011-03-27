
import Qt 4.7
import QtMultimediaKit 1.1

FocusScope {
    property Camera camera
    property bool previewAvailable : false

    signal previewSelected
    id : captureControls

    Column {
        spacing : 8

        Button {
            source: "pics/toolbutton.png"
            fontSize: 20
            width: 105
            height: 50
            opacity: 0.65
            onClicked: {
                if (camera.lockStatus == Camera.Unlocked)
                    camera.searchAndLock();
                else
                    camera.unlock();
            }

            text: {
                if (camera.lockStatus == Camera.Unlocked)
                    "Focus";
                else if (camera.lockStatus == Camera.Searching)
                    "Focusing"
                else
                    "Unlock"
            }
        }

        Button {
            text: "Capture"
            source: "pics/toolbutton.png"
            fontSize: 20
            width: 105
            height: 50
            opacity: 0.65
            onClicked: {
                    camera.flashMode = Camera.FlashAuto;
                    camera.captureImage();
            }
        }

        Button {
            text: "View"
            source: "pics/toolbutton.png"
            fontSize: 20
            width: 105
            height: 50
            opacity: 0.65
            onClicked: captureControls.previewSelected()
            visible: captureControls.previewAvailable
        }
    }

    ZoomControl {
        anchors.left: parent.left
        anchors.right: parent.right
        width : 100
        height: parent.height

        currentZoom: camera.digitalZoom
        maximumZoom: Math.min(4.0, camera.maximumDigitalZoom)
        onZoomTo: camera.setDigitalZoom(value)
    }
}
