import QtQuick 2.1
import QtQuick.Layouts 1.0

Rectangle {
    id: calculator

    property alias displayText: displayText.text
    signal keyClicked(int key)
    signal cutTriggered()
    signal copyTriggered()
    signal pasteTriggered()

    color: "ghostwhite"

    Keys.onPressed: {
        if (event.modifiers & Qt.ControlModifier) {
            if (event.key === Qt.Key_X) {
                cutTriggered();
                event.accepted = true;
            }
            else if (event.key === Qt.Key_C) {
                copyTriggered();
                event.accepted = true;
            }
            else if (event.key === Qt.Key_V) {
                pasteTriggered();
                event.accepted = true;
            }
        }
    }

    // main layout ///////////////////////////////////////////////////////////

    GridLayout {
        id: mainLayout
        columns: 4
        anchors.fill: parent
        anchors.leftMargin: columnSpacing
        anchors.rightMargin: columnSpacing
        anchors.topMargin: rowSpacing
        anchors.bottomMargin: rowSpacing

        Rectangle {
            id: display
            color: "aliceblue"
            border.color: "cadetblue"
            border.width: 2
            height: 56
            radius: 8
            Layout.columnSpan: 4
            Layout.fillWidth: true

            Text {
                id: displayText
                text: "0"
                font.pixelSize: parent.height - 4*2
                font.bold: true
                elide: Text.ElideLeft
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                anchors.rightMargin: 4
            }
        }

        Repeater {
            Button {
                id: key
                text: modelData.text

                Layout.preferredWidth: 48
                Layout.preferredHeight: 40
                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.rowSpan: (modelData.rowSpan !== undefined)? modelData.rowSpan: 1
                Layout.columnSpan: (modelData.columnSpan !== undefined)? modelData.columnSpan: 1

                onClicked: calculator.keyClicked(modelData.key)
            }

            model: [
                { text: "C", key: Qt.Key_Delete },
                { text: "/", key: Qt.Key_Slash },
                { text: "*", key: Qt.Key_Asterisk },
                { text: "-", key: Qt.Key_Minus },

                { text: "7", key: Qt.Key_7 },
                { text: "8", key: Qt.Key_8 },
                { text: "9", key: Qt.Key_9 },
                { text: "+", key: Qt.Key_Plus, rowSpan: 2 },

                { text: "4", key: Qt.Key_4 },
                { text: "5", key: Qt.Key_5 },
                { text: "6", key: Qt.Key_6 },

                { text: "1", key: Qt.Key_1 },
                { text: "2", key: Qt.Key_2 },
                { text: "3", key: Qt.Key_3 },
                { text: "=", key: Qt.Key_Enter, rowSpan: 2 },

                { text: "0", key: Qt.Key_0, columnSpan: 2 },
                { text: ".", key: Qt.Key_Period }
            ]
        }
    }
}
