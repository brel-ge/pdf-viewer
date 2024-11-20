import QtQuick
import ch.brel
import ch.brel.theme

import "../atoms"

Rectangle {
    id: backHomeMenu
    width: 205
    readonly property int spacing: 15
    readonly property int statusBarHeight: 72
    color: "transparent"

    signal homeClicked

    Rectangle {
        id: background
        anchors {
            top: parent.top
            topMargin: backHomeMenu.statusBarHeight

            right: parent.right
            bottom: parent.bottom
        }
        width: 205
        color: Theme.backgroundColor

        PrimaryButton {
            id: mainMenuButton
            anchors {
                top: parent.top
                topMargin: backHomeMenu.spacing
                right: parent.right
                rightMargin: backHomeMenu.spacing
            }
            icon.source: cameraIcon("home")
            onClicked: homeClicked()
        }
    }
}
