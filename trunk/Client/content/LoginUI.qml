import Qt 4.7

Rectangle {
    anchors.fill: parent

    //set backgroud image
    BorderImage {
        id: loginUIViewImage
        source: "pics/background.png"
        anchors.fill: parent
    }

    // minimize button
    Button {
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10
        source: "pics/minimize.png"
        width: 60
        height: 60
        opacity: 0.45
        onClicked: client.minimize()
    }

    // quit button
    Button {
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10
        source: "pics/exit.png"
        width: 60
        height: 60
        opacity: 0.45
        onClicked: Qt.quit()
    }

    // photo login UI
    PhotoLoginUI {
        id: photoLoginUI
    }

    // username lgoin UI
    UsernameLoginUI{
        id: usernameLoginUI
    }

    // login successfully UI
    Image {
        id: loginSuccess
        source: "pics/textArea.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 20
            text: "Welcome"; color: "#FFFFFF"
            font.pixelSize: 90; font.bold: true
            style: Text.Raised; styleColor: "black"
            rotation: 10;
        }

        states: [
            State {
                name: "invisible"
                PropertyChanges { target: loginSuccess; visible: false }
            },
            State {
                name: "visible"
                PropertyChanges { target: loginSuccess; visible: true }
            }
        ]
        transitions: Transition {
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutBounce; duration: 400 }
        }
     }

    // login failed UI
    Image {
        id: loginFail
        source: "pics/textArea.png"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 20
            text: "Login Failed"; color: "#FFFFFF"
            font.pixelSize: 80; font.bold: true
            style: Text.Raised; styleColor: "black"
            rotation:  -10;
        }

        states: [
            State {
                name: "invisible"
                PropertyChanges { target: loginFail; visible: false }
            },
            State {
                name: "visible"
                PropertyChanges { target: loginFail; visible: true }
            }
        ]
        transitions: Transition {
            NumberAnimation { properties: "opacity"; easing.type: Easing.OutBounce; duration: 400 }
        }
     }

    Timer {
         id: loginUITimer
         interval: 500
         onTriggered: {
             loginUI.visible = false
             gameStartUI.visible = true
             client.sendMessage(";GAMELIST;");
         }
     }

    Connections {
            target: client
            onLoginSuccess: {
                photoLoginUI.state = "invisible"
                usernameLoginUI.state = "invisible"
                loginSuccess.state = "visible"
                passwordLoginButton.opacity = 0
                photoLoginButton.opacity = 0
                // trigger timer
                loginUITimer.running = true
            }
    }

    Connections {
            target: client
            onLoginFailed: {
                photoLoginUI.state = "invisible"
                usernameLoginUI.state = "invisible"
                loginFail.state = "visible"
            }
    }

    // choosing login UI between username login and photo login
    Row {
        spacing: 200
        anchors.verticalCenter : parent.verticalCenter
        anchors.horizontalCenter : parent.horizontalCenter
        anchors.verticalCenterOffset: 140

        Button {
            id: passwordLoginButton
            text: "Password Login"
            source: "pics/toolbutton.png"
            fontSize: 20
            width: 200
            height: 50
            opacity: 0.45
            onClicked: usernameLoginUI.toggle()
        }

        Button {
            id: photoLoginButton
            text: "Photo Login"
            source: "pics/toolbutton.png"
            fontSize: 20
            width: 200
            height: 50
            opacity: 0.45
            onClicked: {
                 photoLoginUI.toggle();
//                console.log ("single click accepted")
//               var newObject = Qt.createQmlObject('import QtQuick 1.0; PhotoLoginUI {id: photoLoginUI}',
//                    test, "dynamicSnippet1");
//                newObject.destroy(30000);
            }
        }
    }
}


