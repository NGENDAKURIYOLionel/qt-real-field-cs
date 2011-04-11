import QtQuick 1.0

Column {
    state: "invisible"
    visible: opacity > 0
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    spacing: 5

    Connections {
            target: client
            onLoginFailed: {  photoLoginUI.state = "invisible"
                usernameLoginUI.state = "invisible"
                loginFail.state = "visible"}
        }
    Rectangle { radius: 10.0; opacity:  0.75
                width: 250; height: 48
                LineEdit {
                    id: emailLineEdit
                    fontSize: 22
                    text: "    Your Email"
                    echoMode: TextInput.Normal
                }
    }
    Rectangle { radius: 10.0; opacity:  0.75
                width: 250; height: 48
               LineEdit {
                   id: passwordLineEdit
                   fontSize: 22
                   text: "    Your Password"
                   echoMode: TextInput.Password
               }
    }
    Rectangle { radius: 10.0
                width: 250; height: 50
                color: "transparent"
                Button {
                    id: usernameLoginButton
                    anchors.centerIn: parent
                    source: "pics/toolbutton.png"
                    anchors.verticalCenter : parent.verticalCenter
                    anchors.horizontalCenter : parent.horizontalCenter
                    text: "Login"
                    fontSize: 20
                    width: 105
                    height: 50
                    opacity: 0.65
                    onClicked: client.sendMessage()
                }
    }

    states: [
        State {
            name: "invisible"
            PropertyChanges { target: usernameLoginUI; opacity: 0 }
        },
        State {
            name: "visible"
            PropertyChanges { target: usernameLoginUI; opacity: 1.0 }
        }
    ]
    transitions: Transition {
        NumberAnimation { properties: "opacity"; duration: 400 }
    }
    function toggle() {
        if (state == "visible") {
            state = "invisible";
            photoLoginUI.state = "invisible";
        }
        else {
            state = "visible";
            photoLoginUI.state = "invisible";
        }
    }
}
