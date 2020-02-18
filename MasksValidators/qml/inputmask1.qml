import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 300
    height: 100
    title: qsTr("Input Mask Demo")

    GridLayout {
        columns: 2

        Text {
            id: label1
            text: qsTr("Input Text:")
        }

        TextInput {
            id: input
            text: qsTr("1")
            inputMask: mask.text
            onTextChanged: acceptableInput ? print("Input acceptable") : print("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Input Mask:")
        }

        TextInput {
            id: mask
            text: qsTr("999999")
        }
    }
}
