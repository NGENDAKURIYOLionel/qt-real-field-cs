import QtQuick 1.0
import Qt 4.7
import QtMultimediaKit 1.1
import "content"

Rectangle {
    id : clientUI
    width : 800
    height : 480

   Rectangle {
       id: networkSetting
       width : 800
       height : 480
       color: "black"


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

       Column {
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.verticalCenter: parent.verticalCenter

           spacing: 5

           Text {
               font.pixelSize: 40
               anchors.top: parent.top
               anchors.horizontalCenter: parent.horizontalCenter
               text: "Network Setting"
               color: "white"
           }

           Rectangle { radius: 10.0; opacity:  0.75
                       width: 250; height: 48
                       anchors.horizontalCenter: parent.horizontalCenter
                       LineEdit {
                           id: ipAddress
                           fontSize: 22
                           //text: "    Server IP Address"
                           text: "86.50.129.148"
                           //text: "86.50.132.192"
                           echoMode: TextInput.Normal
                       }
           }
           Rectangle { radius: 10.0; opacity:  0.75
                       width: 250; height: 48
                       anchors.horizontalCenter: parent.horizontalCenter
                      LineEdit {
                          id: portNumber
                          fontSize: 22
                          //text: "    Server Port Number"
                          text:  "8888"
                          echoMode: TextInput.Normal
                      }
           }

           Button {
               text: "Connect"
               source: "pics/toolbutton.png"
               anchors.horizontalCenter: parent.horizontalCenter
               fontSize: 20
               width: 200
               height: 50
               opacity: 0.45
               onClicked: {
                   error.text = "";
                   client.connectto(ipAddress.text, portNumber.text);
               }
           }

           Text {
               id: error
               font.pixelSize: 20
               anchors.bottom: parent.bottom
               text: ""
               color: "blue"
           }
       }
   }

   Connections {
           target: client
           onToGameLogin: {
               networkSetting.visible = false
               loginUI.visible = true
           }
   }

   Connections {
           target: client
           onEmitError: {
               error.text = errorMessage
           }
   }

    LoginUI {
        id:loginUI
        visible: false
    }

    GameStartUI {
        id: gameStartUI
        visible: false
    }

    GameUI {
        id: gameUI
        visible: false
    }

    GameEndUI {
        id: gameEndUI
        visible: false
    }
}
