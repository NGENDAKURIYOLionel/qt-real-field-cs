import QtQuick 1.0

Rectangle {
    id: textArea
    property string text
    property int fontSize
    property color color: "black"
    property alias w: textArea.width
    property alias h: textArea.height
    state: "invisible"
    visible: false
    opacity:  0


    BorderImage {
        id: textAreaImage
        source: "pics/toolbotton.png"
        width: textArea.width; height: textArea.height
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        color: textArea.color
        rotation: 20
        anchors.centerIn: textAreaImage; font.bold: true
        text: textArea.text; style: Text.Raised; styleColor: "black"
        font.pixelSize: textArea.fontSize
    }
    states: [
        State {
            name: "invisible"
            PropertyChanges { target: textArea; visible: false }
        },
        State {
            name: "visible"
            PropertyChanges { target: textArea; visible: true }
        }
    ]
    transitions: Transition {
        NumberAnimation { properties: "opacity"; easing.type: Easing.OutBounce; duration: 400 }
    }
}
