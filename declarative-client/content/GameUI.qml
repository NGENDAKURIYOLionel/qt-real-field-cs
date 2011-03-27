import QtQuick 1.0
import QtMultimediaKit 1.1

Item {
    anchors.fill: parent
    state: "PhotoCapture"

    Camera {
        id: camera
        anchors.right: parent.right
        focus : visible
        captureResolution : "640x480"
    }

    // minimize button
    Button {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        source: "pics/minimize.png"
        width: 60
        height: 60
        opacity: 0.25
        //onClicked: mainWindows.minimizeWindow()
    }

    // quit button
    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 10
        source: "pics/exit.png"
        width: 60
        height: 60
        opacity: 0.25
        onClicked: Qt.quit()
    }

    Image {
        id: scope
        source: "pics/scope.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: battleInfo.width/2
        width: 100
        height: 100
    }

    Rectangle {
        id: battleInfo
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 120
        height: parent
        color: "transparent"
        Text {
            font.pixelSize: 15
            text: "battle info or map"
        }
    }

    Button {
        id: aiming
        source: "pics/triggerButton.png"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 120
        height: 120
        opacity: 0.45
        onClicked: {
                camera.flashMode = Camera.FlashAuto;
                if (camera.lockStatus == Camera.Unlocked){
                   camera.searchAndLock();
                }
                else
                    camera.unlock();
                aiming.visible = false;
                trigger.visible = true;
        }
    }

    Button {
        id: trigger
        visible:  false
        source: "pics/triggerButton.png"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 120
        height: 120
        opacity: 1
        onClicked: {
                camera.captureImage();
                trigger.visible = false;
                aiming.visible = true;
        }
    }


}
