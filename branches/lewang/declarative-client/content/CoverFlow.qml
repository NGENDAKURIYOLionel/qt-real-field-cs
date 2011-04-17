import QtQuick 1.0

Rectangle {
    id: coverFlow

    property int itemWidth: 100
    property int itemHeight: 100

    property ListModel listModel

    signal indexChanged(int index)

    Component {
        id: appDelegate

        Flipable {
            id: myFlipable

            property bool flipped: false

            width: itemWidth; height: itemHeight
            z: PathView.z
            scale: PathView.iconScale

            function itemClicked()
            {
                if(PathView.isCurrentItem) {
                   myFlipable.flipped = !myFlipable.flipped
                   myPathView.interactive = !myFlipable.flipped
                }
                else if(myPathView.interactive) {
                    myPathView.currentIndex = index
                }
            }


            //Keys.onReturnPressed: itemClicked()

            transform: Rotation {
                 id: rotation
                 origin.x: myFlipable.width/2
                 origin.y: myFlipable.height/2
                 axis.x: 0; axis.y: 1; axis.z: 0
                 angle: PathView.angle
             }

            states: State {
                 name: "back"
                 PropertyChanges { target: rotation; angle: 180 }
                 PropertyChanges {target: myFlipable; width: myPathView.width; height: myPathView.height }
                 when: myFlipable.flipped
             }

            transitions: Transition {
                 ParallelAnimation {
                    NumberAnimation { target: rotation; property: "angle"; duration: 250 }
                    NumberAnimation {target: myFlipable; properties: "height,width"; duration: 250}
                 }
             }

            front: Rectangle {
                smooth: true
                width: itemWidth; height: itemHeight
                anchors.centerIn: parent
                color: "black"
                opacity:  0.75
                radius:  25

                Image {
                    id: myIcon
                    anchors.centerIn: parent
                    source: icon
                    smooth: true
                    width: itemWidth-50; height: itemHeight-50
                    Text {
                        id: frontName
                        anchors.centerIn: parent
                        text: name; color: "darkgrey"
                        font.pixelSize: 30; font.bold: true
                        style: Text.Raised; styleColor: "black"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        itemClicked()
                        if (frontName.text != "New") {
                            client.sendMessage(";JOINGAME;" + frontName.text)
                        }
                    }
                }
            }
            back: Rectangle {
                width: itemWidth+400; height: itemHeight+90
                anchors.centerIn: parent
                color: "black"
                opacity:  0.5
                radius:  25

                states:[
                    State {
                        name: "hostGame"
                        when: frontName.text == "New"
                        PropertyChanges {target: hostGameView; visible: true}
                        PropertyChanges {target: listGameView; visible: false}
                    }
//                    State {
//                        name:  "joinGame"
//                        when:  frontName.text != "New"
//                        StateChangeScript {
//                            name: "sendJoinMessage"
//                            script: client.sendMessage(";JOINGAME;" + frontName.text);
//                        }
//                    }
                ]

                Connections {
                        target: client
                        onJoinGameInfo: {
                            if (frontName.text == gameId)
                            console.log("onJoinGameInfo:" + gameId + gameTime + noOfTeamA + noOfTeamB)
                            listGameId.text = "Game: " + gameId
                            listGameNoOfTeamA.text = "Players of Team A: " + noOfTeamA
                            listGameNoOfTeamB.text = "Players of Team B: " + noOfTeamB
                            listGameTime.text = "Time(s): " + gameTime
                        }
                }

                Connections {
                        target: client
                        onGameCreateFailed: {
                            if (frontName.text == "New") {
                                gameCreateFailedText.text = error
                                gameCreateFailedArea.visible = true
                                hostGameView.visible = false
                            }
                        }
                }

                Connections {
                        target: client
                        onGameCreateSuccess: {
                            if (frontName.text == "New") {
                                listGameViewForCreator.visible = true
                            }
                        }
                }

                // game create failed view
                Rectangle {
                    id: gameCreateFailedArea
                    visible: false
                    radius: 10.0
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: parent.width - 100
                    height: parent.height - 100
                    color: "black"

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            gameCreateFailedArea.visible = false
                            hostGameView.visible = true
                        }
                    }

                    Text {
                        id: gameCreateFailedText
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        color: "white"
                    }
                }

                // game create success view for creator to join game
                Item {
                    id: listGameViewForCreator
                    visible: false
                    anchors.fill: parent
                    Text {
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                        color: "white"
                        text: "Game information and setting will be here " + name
                    }

                    Row {
                    spacing: 50
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 140

                    Button {
                        id :gameCratorReady
                        text: "Join"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        onClicked: console.log("in flickable back clicked")
                    }

                    Button {
                        id: gameCratorStart
                        text: "Start"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        onClicked: {
                            gameStartUI.visible = false
                            gameUI.visible = true
                        }
                    }

                    Button {
                        text: "Back"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        MouseArea {
                            anchors.fill: parent
                            onClicked: itemClicked()
                        }
                    }
                }
                }

                // hostGameView
                Item {
                    id: hostGameView
                    anchors.fill:  parent
                    visible: false

                    Button {
                        text: "Game ID"
                        anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: -105
                        anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: -140
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 200
                        height: 70
                    }

                    Button {
                        text: "Time(Seconds)"
                        anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: 15
                        anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: -140
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 200
                        height: 70
                    }

                    Button {
                        text: "NO. of Players of Team A"
                        anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: -105
                        anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: 140
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 200
                        height: 70
                    }

                    Button {
                        text: "NO. of Players of Team B"
                        anchors.verticalCenter: parent.verticalCenter
                            anchors.verticalCenterOffset: 15
                        anchors.horizontalCenter: parent.horizontalCenter
                            anchors.horizontalCenterOffset: 140
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 200
                        height: 70
                    }

                    Rectangle { radius: 10.0; opacity:  0.75
                                width: 250; height: 48
                                anchors.verticalCenter: parent.verticalCenter
                                    anchors.verticalCenterOffset: -60
                                anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.horizontalCenterOffset: -140
                                LineEdit {
                                    id: gameIdEdit
                                    fontSize: 20
                                    text: "  No more than 6 characters"
                                    echoMode: TextInput.Normal
                                }
                    }

                    Rectangle { radius: 10.0; opacity:  0.75
                                width: 250; height: 48
                                anchors.verticalCenter: parent.verticalCenter
                                    anchors.verticalCenterOffset: 60
                                anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.horizontalCenterOffset: -140
                                LineEdit {
                                    id: gameTimeLineEdit
                                    fontSize: 20
                                    text: "  60s < time < 6000s"
                                    echoMode: TextInput.Normal
                                }
                    }

                    Rectangle { radius: 10.0; opacity:  0.75
                                width: 250; height: 48
                                anchors.verticalCenter: parent.verticalCenter
                                    anchors.verticalCenterOffset: -60
                                anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.horizontalCenterOffset: 140
                                LineEdit {
                                    id: teamAPlayerLineEdit
                                    fontSize: 20
                                    text: "    Maximun Player: 20"
                                    echoMode: TextInput.Normal
                                }
                    }

                    Rectangle { radius: 10.0; opacity:  0.75
                                width: 250; height: 48
                                anchors.verticalCenter: parent.verticalCenter
                                    anchors.verticalCenterOffset: 60
                                anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.horizontalCenterOffset: 140
                                LineEdit {
                                    id: teamBPlayerLineEdit
                                    fontSize: 20
                                    text: "   Maximun Player: 20"
                                    echoMode: TextInput.Normal
                                }
                    }

                    Row {
                    spacing: 50
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 140

                    Button {
                        text: "Create"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        onClicked: {
                            client.sendMessage(";CREATEGAME;" + gameIdEdit.text + ";"
                                               + gameTimeLineEdit.text + ";"
                                               + teamAPlayerLineEdit.text + ";"
                                               + teamBPlayerLineEdit.text)
                        }
                   }

                    Button {
                        text: "Back"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        MouseArea {
                            anchors.fill: parent
                            onClicked: itemClicked()
                        }
                    }
                }
                }

                // player (not creator) join view
                Item {
                    id: listGameView
                    anchors.fill: parent

                    Column {
                        spacing: 10
                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: -50
                        anchors.verticalCenterOffset: -60


                        Text {
                            id: listGameId
                            color: "white"
                            //text: "Game ID: "
                        }

                        Text {
                            id: listGameTime
                            color: "white"
                            //text: "Game Time: "
                        }

                        Text {
                            id: listGameNoOfTeamA
                            color: "white"
                            //text: "Players of Team A:  "
                        }

                        Text {
                            id: listGameNoOfTeamB
                            color: "white"
                            //text: "Players of Team B:  "
                        }
                    }



                    Row {
                    spacing: 50
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 70

                        Button {
                            id :joinTeamA
                            text: "Join Team A"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100

//                            states: State {
//                                         name: "down"; when: mouseArea.pressed == true
//                                         PropertyChanges { target: joinTeamA; color: "red"
//                                         }
//                                         PropertyChanges { target: joinTeamB; color: "white"
//                                         }
//                                     }


                            onClicked: {
                                console.log("joinTeamA")
                                joinTeamA.enabled = false
                                joinTeamA.opacity = 0.5

                                joinTeamB.enabled = true
                                joinTeamB.opacity = 1

                                gameReady.enabled = true
                                gameReady.opacity = 1.0
                            }
                        }

                        Button {
                            id: joinTeamB
                            text: "Join Team B"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100

//                            states: State {
//                                         name: "down"; when: mouseArea.pressed == true
//                                         PropertyChanges { target: joinTeamB; color: "red"
//                                         }
//                                         PropertyChanges { target: joinTeamA; color: "white"
//                                         }
//                                     }

                            onClicked: {
                                console.log("joinTeamB")
                                joinTeamB.enabled = false
                                joinTeamB.opacity = 0.5


                                joinTeamA.enabled = true
                                joinTeamA.opacity = 1


                                gameReady.enabled = true
                                gameReady.opacity = 1.0
                            }
                        }

                    }





                    Row {
                    spacing: 50
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 140

                        Button {
                            id :gameReady
                            enabled: false
                            opacity: 0.1
                            text: "Ready"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100
                            onClicked: {
                                console.log("ready")
                                if (joinTeamA.enabled)
                                    client.sendMessage(";JOINTEAM;teamB")

                                else
                                    client.sendMessage(";JOINTEAM;teamA")
                            }
                        }

                        Button {
                            text: "Back"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100
                            MouseArea {
                                anchors.fill: parent
                                onClicked: itemClicked()
                            }
                        }
                   }
                }
            }
        }
    }

    PathView {
        id: myPathView

        Keys.onRightPressed: if (!moving && interactive) incrementCurrentIndex()
        Keys.onLeftPressed: if (!moving && interactive) decrementCurrentIndex()

        anchors.fill: parent
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
        interactive: true
        model: listModel
        delegate: appDelegate
        path: Path {
            startX: 0
            startY: coverFlow.height / 2
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: 60 }
            PathAttribute { name: "iconScale"; value: 0.5 }
            PathLine { x: coverFlow.width / 2; y: coverFlow.height / 2;  }
            PathAttribute { name: "z"; value: 100 }
            PathAttribute { name: "angle"; value: 0 }
            PathAttribute { name: "iconScale"; value: 1.0 }
            PathLine { x: coverFlow.width; y: coverFlow.height / 2; }
            PathAttribute { name: "z"; value: 0 }
            PathAttribute { name: "angle"; value: -60 }
            PathAttribute { name: "iconScale"; value: 0.5 }
        }
    }

    Component.onCompleted: {
        myPathView.currentIndexChanged.connect(function(){
            indexChanged(myPathView.currentIndex);
        })
    }
}
