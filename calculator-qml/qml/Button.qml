import QtQuick 2.0
import QtQuick.Layouts 1.0

Rectangle {
    id: button

    Layout.minimumWidth: 32
    Layout.minimumHeight: 32
    Layout.preferredWidth: 128
    Layout.preferredHeight: 32
    Layout.fillWidth: true
    Layout.fillHeight: false
    radius: Math.min(height, width) / 6

    signal clicked()

    property alias text: label.text
    property string startColor: "lightblue"
    property string endColor: "cadetblue"

    gradient:
        Gradient {
            GradientStop { position: 0.0; color: mousearea.pressed ? button.endColor : button.startColor }
            GradientStop { position: 1.0; color: mousearea.pressed ? button.startColor : button.endColor }
        }

    Text {
        id: label
        anchors.centerIn: parent
        anchors.margins: 4
        font.pixelSize: Math.min(button.height, button.width) - 4*2
        font.bold: true
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        onClicked: {
            button.clicked();
        }
    }
}
