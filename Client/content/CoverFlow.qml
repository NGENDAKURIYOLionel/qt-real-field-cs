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
                            listGameView.visible = true
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
                ]


                Connections {
                        target: client
                        onJoinGameInfo: {
                            if (frontName.text == "New" && !isGameInList) {
                                listGameIdForCreator.text = "Game: " + gameId
                                listGameNoOfTeamAForCreator.text = "Players of Team A: " + noOfTeamA
                                listGameNoOfTeamBForCreator.text = "Players of Team B: " + noOfTeamB
                                listGameTimeForCreator.text = "Time(s): " + gameTime
                                listUserForCreator.text = joinUserName + " joins this game"
                            }
                            if (frontName.text == gameId) {
                                listGameId.text = "Game: " + gameId
                                listGameNoOfTeamA.text = "Players of Team A: " + noOfTeamA
                                listGameNoOfTeamB.text = "Players of Team B: " + noOfTeamB
                                listGameTime.text = "Time(s): " + gameTime
                                listUser.text = joinUserName + " joins this game"
                            }
                        }
                }

                Connections {
                        target: client
                        onLeaveGameInfo: {
                            if (frontName.text == "New" && !isGameInList) {
                                listGameIdForCreator.text = "Game: " + gameId
                                listGameNoOfTeamAForCreator.text = "Players of Team A: " + noOfTeamA
                                listGameNoOfTeamBForCreator.text = "Players of Team B: " + noOfTeamB
                                listGameTimeForCreator.text = "Time(s): " + gameTime
                                listUserForCreator.text = leaveUserName + " leaves this game"
                            }
                            if (frontName.text == gameId) {
                                listGameId.text = "Game: " + gameId
                                listGameNoOfTeamA.text = "Players of Team A: " + noOfTeamA
                                listGameNoOfTeamB.text = "Players of Team B: " + noOfTeamB
                                listGameTime.text = "Time(s): " + gameTime
                                listUser.text = leaveUserName + " leaves this game"
                            }
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
                                listGameIdForCreator.text = "Game: " + gameId
                                listGameNoOfTeamAForCreator.text = "Players of Team A: " + noOfTeamA
                                listGameNoOfTeamBForCreator.text = "Players of Team B: " + noOfTeamB
                                listGameTimeForCreator.text = "Time(s): " + gameTime

                                listGameViewForCreator.visible = true
                                hostGameView.visible = false
                            }
                        }
                }

                Connections {
                        target: client
                        onStartGame: {
                            itemClicked()

                            listGameViewForCreator.visible = false
                            hostGameView.visible = true
                            joinTeamAForCreator.enabled = true
                            joinTeamAForCreator.opacity = 1
                            joinTeamBForCreator.enabled = true
                            joinTeamBForCreator.opacity = 1
                            gameReadyForCreator.enabled = false
                            gameReadyForCreator.opacity = 0.25
                            gameStartForCreator.enabled = false
                            gameStartForCreator.opacity = 0.25
                            listUserForCreator.text = ""

                            listUser.text = ""
                            gameReady.enabled = false
                            gameReady.opacity = 0.25
                            joinTeamA.enabled = true
                            joinTeamA.opacity = 1.0
                            joinTeamB.enabled = true
                            joinTeamB.opacity = 1.0
                            listGameView.visible = false
                        }
                }


//                Connections {
//                        target: client
//                        onGameAbort: {
//                                listGameViewForCreator.visible = false
//                                hostGameView.visible = false
//                                joinTeamAForCreator.enabled = true
//                                joinTeamAForCreator.opacity = 1
//                                joinTeamBForCreator.enabled = true
//                                joinTeamBForCreator.opacity = 1
//                                gameReadyForCreator.enabled = true
//                                gameReadyForCreator.opacity = 1
//                                itemClicked()
//                        }
//                }

                Connections {
                    target: client
                    onTeamJoined: {
                        if (frontName.text == "New" && !isGameInList) {
                            console.log("creator teamJoined")
                            console.log(frontName.text)
                            console.log(gameId)

                            gameReadyForCreator.enabled = false
                            gameReadyForCreator.opacity = 0.5
                        }
                        if (frontName.text == gameId) {
                            console.log("normal user teamJoined")
                            console.log(frontName.text)
                            console.log(gameId)

                            gameReady.enabled = false
                            gameReady.opacity = 0.5
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
                    Column {
                        spacing: 6
                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: -50
                        anchors.verticalCenterOffset: -60


                        Text {
                            id: listGameIdForCreator
                            color: "white"
                        }

                        Text {
                            id: listGameTimeForCreator
                            color: "white"
                        }

                        Text {
                            id: listGameNoOfTeamAForCreator
                            color: "white"
                        }

                        Text {
                            id: listGameNoOfTeamBForCreator
                            color: "white"
                        }

                        Text {
                            id: listUserForCreator
                            color: "white"
                        }
                    }

                    Row {
                        spacing: 50
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 70

                        Button {
                            id :joinTeamAForCreator
                            text: "Join Team A"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100

                            onClicked: {
                                joinTeamAForCreator.enabled = false
                                joinTeamAForCreator.opacity = 0.5

                                joinTeamBForCreator.enabled = true
                                joinTeamBForCreator.opacity = 1

                                gameReadyForCreator.enabled = true
                                gameReadyForCreator.opacity = 1.0
                            }
                        }

                        Button {
                            id: joinTeamBForCreator
                            text: "Join Team B"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100

                            onClicked: {
                                joinTeamBForCreator.enabled = false
                                joinTeamBForCreator.opacity = 0.5

                                joinTeamAForCreator.enabled = true
                                joinTeamAForCreator.opacity = 1

                                gameReadyForCreator.enabled = true
                                gameReadyForCreator.opacity = 1.0
                            }
                        }

                    }

                    Row {
                        spacing: 25
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 140

                        Button {
                            id :gameReadyForCreator
                            enabled: false
                            opacity: 0.25
                            text: "Ready"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100
                            onClicked: {
                                if (joinTeamAForCreator.enabled)
                                    client.sendMessage(";JOINTEAM;teamB")
                                else
                                    client.sendMessage(";JOINTEAM;teamA")
                                gameStartForCreator.enabled = true
                                gameStartForCreator.opacity = 1.0
                            }
                        }

                        Button {
                            id: gameStartForCreator
                            text: "Start"
                            enabled: false
                            opacity: 0.25
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
//                                    itemClicked()
//                                    listGameViewForCreator.visible = false
//                                    hostGameView.visible = true
//                                    joinTeamAForCreator.enabled = true
//                                    joinTeamAForCreator.opacity = 1
//                                    joinTeamBForCreator.enabled = true
//                                    joinTeamBForCreator.opacity = 1
//                                    gameReadyForCreator.enabled = false
//                                    gameReadyForCreator.opacity = 0.25
//                                    gameStartForCreator.enabled = false
//                                    gameStartForCreator.opacity = 0.25
//                                    listUserForCreator.text = ""
                                    client.sendMessage(";GAMESTART;")
                                }
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
                                onClicked: {
                                    listGameViewForCreator.visible = false
                                    hostGameView.visible = true
                                    joinTeamAForCreator.enabled = true
                                    joinTeamAForCreator.opacity = 1
                                    joinTeamBForCreator.enabled = true
                                    joinTeamBForCreator.opacity = 1
                                    gameReadyForCreator.enabled = false
                                    gameReadyForCreator.opacity = 0.25
                                    gameStartForCreator.enabled = false
                                    gameStartForCreator.opacity = 0.25
                                    itemClicked()
                                    listUserForCreator.text = ""
                                    client.sendMessage(";LEAVEGAME;" + frontName.text)
                                }
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
                        spacing: 6
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
                        }

                        Text {
                            id: listGameNoOfTeamB
                            color: "white"
                        }

                        Text {
                            id: listUser
                            color: "white"
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

                            onClicked: {
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

                            onClicked: {
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
                            opacity: 0.25
                            text: "Ready"
                            source: "pics/toolbutton1.png"
                            fontSize: 20
                            width: 150
                            height: 100
                            onClicked: {
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
                                onClicked: {
                                    itemClicked()
                                    listUser.text = ""
                                    gameReady.enabled = false
                                    gameReady.opacity = 0.25
                                    joinTeamA.enabled = true
                                    joinTeamA.opacity = 1.0
                                    joinTeamB.enabled = true
                                    joinTeamB.opacity = 1.0
                                    client.sendMessage(";LEAVEGAME;" + frontName.text)
                                }
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
