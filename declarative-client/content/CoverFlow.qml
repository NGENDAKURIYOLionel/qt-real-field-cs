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
                        anchors.centerIn: parent
                        text: name; color: "darkgrey"
                        font.pixelSize: 70; font.bold: true
                        style: Text.Raised; styleColor: "black"
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: itemClicked()
                }
            }
            back: Rectangle {
                width: itemWidth+400; height: itemHeight+90
                anchors.centerIn: parent
                color: "black"
                opacity:  0.5
                radius:  25

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
                        id :gameReady
                        text: "Ready"
                        source: "pics/toolbutton1.png"
                        fontSize: 20
                        width: 150
                        height: 100
                        onClicked: console.log("in flickable back clicked")
                    }

                    Button {
                        id: gameStart
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
