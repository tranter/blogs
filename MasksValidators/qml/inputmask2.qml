import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.4

ApplicationWindow {
    visible: true
    width: 300
    height: 200
    title: qsTr("Input Mask Demo")
    footer: ToolBar {
        Label {
            Text {
                id: statusLine
            }
        }
    }

    GridLayout {
        columns: 2

        Text {
            id: label1
            text: qsTr("Input Text:")
        }

        TextField {
            id: input
            text: qsTr("1")
            inputMask: mask.text
            onTextChanged: acceptableInput ? statusLine.text = qsTr("Input acceptable") : statusLine.text = qsTr("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Input Mask:")
        }

        TextField {
            id: mask
            text: qsTr("999999")
        }

        Button {
            text: qsTr("&Clear")
            onClicked: {
                input.clear()
                mask.clear()
            }
        }
        Button {
            text: qsTr("&Quit")
            onClicked: {
                Qt.quit()
            }
        }
    }
}
