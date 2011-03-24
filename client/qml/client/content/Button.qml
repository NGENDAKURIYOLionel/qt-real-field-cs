import QtQuick 1.0

Item {
    id: button
    signal clicked

    property string text
    property color color: "white"
    property int fontSize
    property alias w: button.width
    property alias h: button.height


    BorderImage {
        id: buttonImage
        source: "pics/toolbutton.png"
        width: button.width; height: button.height
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: buttonImage

        onPressed: {
            buttonImage.anchors.right = parent.right
            buttonImage.anchors.rightMargin = -1
            buttonImage.anchors.bottom = parent.bottom
            buttonImage.anchors.bottomMargin = -1
         }
        onReleased: {
            buttonImage.anchors.right = parent.right
            buttonImage.anchors.rightMargin = 1
            buttonImage.anchors.bottom = parent.bottom
            buttonImage.anchors.bottomMargin = 1
        }
        onClicked:  {
           console.log ("single click accepted")
           button.clicked();
        }
    }

    Text {
        id: buttonText
        color: button.color
        anchors.centerIn: buttonImage; font.bold: true
        text: button.text; style: Text.Raised; styleColor: "black"
        font.pixelSize: button.fontSize
    }
}
