import Qt 4.7

Item {
    //id: gameEndtUI
    anchors.fill: parent
    visible: false

    //set backgroud image
    BorderImage {
        id: lgameStartUIViewImage
        source: "pics/gameStartBackGround.jpg"
        width: gameStartUI.width; height: gameStartUI.height
    }

    Button {
        text: "Game Summary"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        source: "pics/toolbutton1.png"
        fontSize: 25
        width: 200
        height: 70
        onClicked: {
            console.log(winner);
        }
    }

    Button {
        text: "Back To Game Lobby"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        source: "pics/toolbutton1.png"
        fontSize: 25
        width: 200
        height: 70
        onClicked: {
            gameEndUI.visible = false
            //usernameLoginUI.state = "invisible"
            loginSuccess.state = "visible"
            //passwordLoginButton.opacity = 0
            //photoLoginButton.opacity = 0
            // trigger timer
            loginUITimer.running = true


        }
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



    Button {
        text: "Choose a New Game"
        source: "pics/toolbutton1.png"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        fontSize: 20
        width: 200
        height: 70
        //onClicked: photoLoginUI.toggle()
    }



    LineEdit {
        fontSize: 22
        text: " bla bla"
        echoMode: TextInput.Normal
    }
}
