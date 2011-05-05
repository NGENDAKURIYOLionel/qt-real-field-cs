import Qt 4.7

Item {
    anchors.fill: parent
    visible: false

    //set backgroud image
    BorderImage {
        id: lgameStartUIViewImage
        source: "pics/gameStartBackGround.jpg"
        opacity: 0.9
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
    }

    Connections {
            target: client
            onShowResult: {
                result.text = winner
            }
    }

    Text {
        id: result
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 20
        color: "#EEEEEE"
        font.pixelSize: 90; font.bold: true
        style: Text.Raised; styleColor: "black"
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
            gameStartUI.visible = true
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
        opacity: 0.55
        onClicked: client.minimize()
    }

    // quit button
    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 10
        source: "pics/exit.png"
        width: 60
        height: 60
        opacity: 0.55
        onClicked: Qt.quit()
    }
}
