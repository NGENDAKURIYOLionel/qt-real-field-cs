import QtQuick 1.0
import QtMultimediaKit 1.1

Item {
    anchors.fill: parent
    state: "PhotoCapture"

    Camera {
        id: cameraShoot
        anchors.right: parent.right
        focus : invisible
        captureResolution : "850x480"
    }

    Text {
        id: battleInfo
        font.pixelSize: 16
        anchors.bottom: parent.bottom
        text: "Battle Info: "
        color: "blue"
    }

    Text {
        id: gameTime
        font.pixelSize: 20
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: "Time: "
        color: "blue"
    }

    Connections {
            target: client
            onOnTarget: {
                if (hit == false)
                    battleInfo.text = "Battle Info: You missed your target"
                else
                    battleInfo.text = "Battle Info: You are right on target " + uName
            }
    }

    Connections {
            target: client
            onGameUpdate: {
                if (isSelfKilled == "true") {
                   battleInfo.text = "Battle Info: " + " TeamA: " + noOfTeamALeft
                                   + " TeamB: " + noOfTeamBLeft + "   " + shooter  + " shot at "
                                   + beShotOne + " Health:" + health + " Your Status: " + "alive"
                }
                else {
                    battleInfo.text = "Battle Info: " + " TeamA: " + noOfTeamALeft
                                    + " TeamB: " + noOfTeamBLeft + "   " + shooter  + " shot at "
                                    + beShotOne + " Health:" + health + " Your Status: " + "dead"
                    aiming.enabled = false
                    aiming.opacity = 0.15
                    trigger.enabled = false
                    aiming.opacity = 0.15
                    trigger.enabled = false
                    trigger.visible = false
                }
            }
    }

    Connections {
            target: client
            onGameTime: {
                gameTime.text = "Time: " + remainingTime
            }
    }

    Connections {
            target: client
            onGameEnd: {
                gameUI.visible = false
                gameEndUI.visible = true
                cameraShoot.stop
            }
    }

    // test buttons
//    Row {
//        spacing: 20
//        anchors.centerIn: parent

//        Button {
//            text: "gameUpdate1"
//            source: "pics/toolbutton1.png"
//            fontSize: 25
//            width: 150
//            height: 70
//            onClicked: {
//                client.sendMessage(";GAMEUPDATE1;")
//            }
//        }

//        Button {
//            text: "gameUpdate2"
//            source: "pics/toolbutton1.png"
//            fontSize: 25
//            width: 150
//            height: 70
//            onClicked: {
//                client.sendMessage(";GAMEUPDATE2;")
//            }
//        }

//            Button {
//                text: "onTarget1"
//                source: "pics/toolbutton1.png"
//                fontSize: 25
//                width: 150
//                height: 70
//                onClicked: {
//                    client.sendMessage(";ONTARGET1;")
//                }
//            }

//            Button {
//                text: "onTarget2"
//                source: "pics/toolbutton1.png"
//                fontSize: 25
//                width: 150
//                height: 70
//                onClicked: {
//                    client.sendMessage(";ONTARGET2;")
//                }
//            }

//            Button {
//                text: "endGame"
//                source: "pics/toolbutton1.png"
//                fontSize: 25
//                width: 150
//                height: 70
//                onClicked: {
//                    client.sendMessage(";ENDGAME;")
//                }
//            }
//        }


    // minimize button
    Button {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 10
        source: "pics/minimize.png"
        width: 40
        height: 40
        opacity: 0.55
        onClicked: client.minimize()
    }

    // quit button
    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 10
        source: "pics/exit.png"
        width: 40
        height: 40
        opacity: 0.55
        onClicked: Qt.quit()
    }

    Image {
        id: scope
        source: "pics/scope.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: -30
        width: 100
        height: 100
     }

    Button {
        id: aiming
        source: "pics/triggerButton.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 150
        anchors.right: parent.right
        width: 120
        height: 120
        opacity: 0.45
        onClicked: {
                if (cameraShoot.lockStatus == Camera.Unlocked){
                   cameraShoot.searchAndLock();
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
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 150
        anchors.right: parent.right
        width: 120
        height: 120
        opacity: 1
        onClicked: {
                //camera.flashMode = Camera.FlashAuto;
                cameraShoot.flashMode = Camera.FlashOff;
                cameraShoot.captureImage();
                client.sendImage("")
                trigger.visible = false;
                aiming.visible = true;
        }
    }
}
