import Qt 4.7

Item {
    id: gameStartUI
    anchors.fill: parent
    visible: false

    //set backgroud image
    BorderImage {
        id: lgameStartUIViewImage
        source: "pics/gameStartBackGround.jpg"
        width: gameStartUI.width; height: gameStartUI.height
    }


    Button {
        text: "Choose a Game to Join"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        source: "pics/toolbutton1.png"
        fontSize: 25
        width: 200
        height: 70
    }

    // coverflow
    Item {
        width: parent.width; height: 400
        anchors.verticalCenter: parent.verticalCenter

        ListModel {
            id: appModel
            ListElement { name: "4";  icon: "pics/numberIcon.png" }
            ListElement { name: "10"; icon: "pics/numberIcon.png" }
            ListElement { name: "8";  icon: "pics/numberIcon.png" }
            ListElement { name: "12"; icon: "pics/numberIcon.png" }
            ListElement { name: "0";  icon: "pics/numberIcon.png" }
        }

        CoverFlow {
            listModel: appModel
            color: "transparent"
            anchors.fill:  parent

//            Text {
//                id: myText
//                anchors.bottom: parent.bottom
//                text: "current"
//                anchors.horizontalCenter: parent.horizontalCenter
//            }
//            onIndexChanged: {
//                myText.text = "Current index: " + index
//            }

            itemWidth: 250
            itemHeight: 250
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

}
