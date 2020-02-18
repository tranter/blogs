import QtQuick 2.6
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    width: 400
    height: 400

    property int fontSize: Qt.application.font.pixelSize * 1.5

    header: Label {
        text: qsTr("RegExpValidator")
        font.bold: true
        font.pixelSize: Qt.application.font.pixelSize * 2
        horizontalAlignment: Text.AlignHCenter
        padding: 10
    }

    GridLayout {
        columns: 2

        Text {
            id: label1
            font.pixelSize: fontSize
            text: qsTr("Input Text:")
        }

        TextField {
            id: input
            text: qsTr("1")
            font.pixelSize: fontSize
            validator: RegExpValidator {
                id: validator
                regExp: /[a-zA-Z0-9]+/
            }
            onTextChanged: acceptableInput ? status.text = qsTr("Input acceptable") : status.text = qsTr("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Regular Expression:")
            font.pixelSize: fontSize
        }

        TextField {
            id: regexp
            text: "[a-zA-Z0-9]+"
            font.pixelSize: fontSize
            onTextChanged: validator.regExp = RegExp(text)
        }

        Text {
            id: label4
            text: qsTr("Status:")
            font.pixelSize: fontSize
            Layout.topMargin: 16
        }

        Text {
            id: status
            font.pixelSize: fontSize
            font.bold: true
            Layout.topMargin: 16
        }
    }
}
