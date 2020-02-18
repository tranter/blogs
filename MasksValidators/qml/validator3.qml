import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0

Window {
    visible: true
    width: 300
    height: 100
    title: qsTr("Regular Expression Validator Demo")

    GridLayout {
        columns: 2

        Text {
            id: label1
            text: qsTr("Input Text:")
        }

        TextInput {
            id: input
            text: qsTr("1")
            validator: RegExpValidator {
                regExp: RegExp(regexp.text)
            }
            onTextChanged: acceptableInput ? print("Input acceptable") : print("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Regular Expression:")
        }

        TextInput {
            id: regexp
            text: "[a-zA-Z0-9]+"
        }
    }
}
