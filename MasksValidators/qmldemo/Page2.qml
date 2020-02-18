import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.3

Page {
    width: 400
    height: 400

    property int fontSize: Qt.application.font.pixelSize * 1.5

    header: Label {
        text: qsTr("DoubleValidator")
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
            validator: DoubleValidator {
                bottom: parseInt(bottom.text)
                top: parseInt(top.text)
                decimals: parseInt(decimals.text)
            }
            onTextChanged: acceptableInput ? status.text = qsTr("Input acceptable") : status.text = qsTr("Input not acceptable")
        }

        Text {
            id: label2
            text: qsTr("Bottom:")
            font.pixelSize: fontSize
        }

        TextField {
            id: bottom
            inputMask: "00000000"
            text: qsTr("1")
            font.pixelSize: fontSize
        }

        Text {
            id: label3
            text: qsTr("Top:")
            font.pixelSize: fontSize
        }

        TextField {
            id: top
            inputMask: "00000000"
            text: qsTr("100")
            font.pixelSize: fontSize
        }

        Text {
            id: label5
            text: qsTr("Decimals:")
            font.pixelSize: fontSize
        }

        TextField {
            id: decimals
            inputMask: "00"
            text: qsTr("2")
            font.pixelSize: fontSize
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
