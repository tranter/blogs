import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 300
    height: 100
    title: qsTr("Int Validator Demo")

    GridLayout {
        columns: 2

        Text {
            id: label1
            text: qsTr("Input Text:")
        }

        TextInput {
            id: input
            text: qsTr("1")
            validator: IntValidator {
                bottom: parseInt(bottom.text)
                top: parseInt(top.text)
            }
            onTextChanged: acceptableInput ? print("Input acceptable") : print("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Bottom:")
        }

        TextInput {
            id: bottom
            inputMask: "00000000"
            text: qsTr("1")
        }

        Text {
            id: label3
            text: qsTr("Top:")
        }

        TextInput {
            id: top
            inputMask: "00000000"
            text: qsTr("100")
        }
    }
}
