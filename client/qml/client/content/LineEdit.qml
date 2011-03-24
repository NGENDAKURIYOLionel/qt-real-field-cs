 import Qt 4.7

 Rectangle {
     property string text: ""
     property int fontSize
     property int echoMode

  //   signal textChanged(string text)

     id: lineEdit
     radius: 10.0
     anchors.fill: parent
     color: "transparent"

     TextInput {
         id: input
         font.pixelSize: lineEdit.fontSize
         anchors.verticalCenter: lineEdit.verticalCenter
         anchors.left:  lineEdit.left
         text: lineEdit.text
         color: "grey"
     }

     MouseArea {
         anchors.fill: parent
         onClicked: {
             input.focus = true
             lineEdit.color = "white"
             input.color = "black"
             input.text = ""
             input.echoMode = echoMode
         }
     }

//     Keys.onReturnPressed: {
//         lineEdit.textChanged(input.text)
//         lineEdit.focus = true
//     }
//     Keys.onEnterPressed: {
//         lineEdit.textChanged(input.text)
//         lineEdit.focus = true
//     }
//     Keys.onEscapePressed: {
//         lineEdit.textChanged(input.text)
//         lineEdit.focus = true
//     }
 }
