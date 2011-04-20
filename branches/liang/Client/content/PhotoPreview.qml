import Qt 4.7
import QtMultimediaKit 1.1

Item {
    property alias source : preview.source
    signal closed

    Image {
        id: preview
        anchors.fill : parent
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            parent.closed();
        }
    }
}

