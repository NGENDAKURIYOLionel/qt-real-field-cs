import Qt 4.7

Item {
    anchors.fill: parent
    visible: false

    //set backgroud image
    BorderImage {
        id: gameStartUIViewImage
        source: "pics/gameStartBackGround.jpg"
        width: gameStartUI.width; height: gameStartUI.height
    }


    Button {
        text: "Create a New Game or Choose a Game to Join"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        source: "pics/toolbutton1.png"
        fontSize: 25
        width: 200
        height: 70
    }

    Connections {
            target: client
            onGameList: {
           // var list = new Array();
            for (var i =0; i < size; i++)
             // Qt.createQmlObject('import Qt 4.7; ListElement { name: "' + list[i] + '"; icon:""pics/numberIcon.png" }', parent, "somefile")
              appModel.append( { name: list[i]  ,icon: "pics/numberIcon.png" })
            }
    }

    // coverflow
    Item {
        id:listModel
        width: parent.width; height: 400
        anchors.verticalCenter: parent.verticalCenter

        ListModel {
            id: appModel
            ListElement { name: "New"  ; icon: "pics/numberIcon.png" }
        }

        CoverFlow {
            listModel: appModel
            color: "transparent"
            anchors.fill:  parent
            itemWidth: 250
            itemHeight: 250
        }

    }

    Connections {
            target: client
            onStartGame: {
                gameStartUI.visible = false
                gameUI.visible = true
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
