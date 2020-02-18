import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 300
    height: 100
    title: qsTr("Double Validator Demo")

    GridLayout {
        columns: 2

        Text {
            id: label1
            text: qsTr("Input Text:")
        }

        TextInput {
            id: input
            text: qsTr("1")
            validator: DoubleValidator {
                bottom: parseInt(bottom.text)
                top: parseInt(top.text)
                decimals: parseInt(decimals.text)
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

        Text {
            id: label4
            text: qsTr("Decimals:")
        }

        TextInput {
            id: decimals
            inputMask: "00"
            text: qsTr("2")
        }
        
    }
}
