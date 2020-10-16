// Simple QML Timer example

import QtQuick 2.6
import QtQuick.Controls 2.4

Rectangle {
    height: 200
    width: 250

    property int count: 0

    Timer {
      id: timer
      interval: 100
      running: false
      repeat: true
      onTriggered: count += 1

    }
    
    Column {
        anchors.fill: parent
        Text {
            id: text
            font.pixelSize: 100
            text: (count / 10).toFixed(1)
        }
        Row {
            spacing: 10

            Button {
                text: timer.running ? "Stop" : "Start"
                onClicked: timer.running = !timer.running
            }
            Button {
                text: "Reset"
                onClicked: count = 0
            }
        }
    }
}
