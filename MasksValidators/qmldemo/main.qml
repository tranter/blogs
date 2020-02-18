import QtQuick 2.6
import QtQuick.Controls 2.1

ApplicationWindow {
    visible: true
    width: 400
    height: 480
    title: qsTr("Validator Demo")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
        }

        Page2 {
        }

        Page3 {
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("IntValidator")
        }

        TabButton {
            text: qsTr("DoubleValidator")
        }

        TabButton {
            text: qsTr("RegExpValidator")
        }
    }
}
