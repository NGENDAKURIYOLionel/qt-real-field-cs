import QtQuick 1.0
import Qt 4.7
//import QtMultimediaKit 1.1
import "content"

Rectangle {
    id : clientUI
    width : 800
    height : 480

    LoginUI {

    }

//    state: "gameLogin"


//    states: [
//            State {
//                name: "gameLogin"
//                StateChangeScript {
//                    script: {
//                        gameLogin.visible = true
//                        gameStart.visible = false
//                    }
//                }
//            },
//            State {
//                name: "gameStart"
//                StateChangeScript {
//                    script: {
//                      gameLogin.visible = false
//                      gameStart.visible = true
//                    }
//                }
//            }
//        ]

}
