import Qt 4.7

Rectangle {
    id: loginUI
    anchors.fill: parent

    BorderImage {
        id: loginUIViewImage
        source: "pics/backgroud_2.jpeg"
        width: loginUI.width; height: loginUI.height
    }

    Column {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        spacing: 5

        Rectangle { radius: 10.0; opacity:  0.75
                    width: 250; height: 48
                    LineEdit {
                        id: emailLineEdit
                        fontSize: 22
                        text: "Your Email Here"
                        echoMode: TextInput.Normal
                    }
        }
        Rectangle { radius: 10.0; opacity:  0.75
                    width: 250; height: 48
                   LineEdit {
                       id: passwordLineEdit
                       fontSize: 22
                       text: "Your Password Here"
                       echoMode: TextInput.Password

                   }
        }
        Rectangle { radius: 10.0
                    width: 250; height: 50
                    color: "transparent"
                    Button {
                        id: loginButton
                        anchors.centerIn: parent
                        anchors.verticalCenter : parent.verticalCenter
                        anchors.horizontalCenter : parent.horizontalCenter
                        text: "Login"
                        fontSize: 20
                        width: 105
                        height: 50
                        opacity: 0.45
                        onClicked: Qt.quit() } }
    }





    Row {
        spacing: 200
        anchors.verticalCenter : parent.verticalCenter
        anchors.horizontalCenter : parent.horizontalCenter
        anchors.verticalCenterOffset: 125

        Button {
            id: passwordLoginButton
            text: "Password Login"
            fontSize: 20
            width: 200
            height: 50
            opacity: 0.45
            onClicked: Qt.quit()
        }

        Button {
            id: photoLoginButton
            text: "Photo Login"
            fontSize: 20
            width: 200
            height: 50
            opacity: 0.45
            //onClicked:
        }

    }
}
