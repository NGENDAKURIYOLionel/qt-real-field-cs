import QtQuick 1.0
import Qt 4.7
import QtMultimediaKit 1.1
import "content"

Rectangle {
    id : clientUI
    width : 800
    height : 480

    LoginUI {
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
