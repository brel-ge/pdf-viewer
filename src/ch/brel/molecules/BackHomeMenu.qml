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
    signal exitClicked

    Rectangle {
        id: background
        anchors {
            top: parent.top
            topMargin: backHomeMenu.statusBarHeight + 15

            right: parent.right
            bottom: parent.bottom
        }
        width: 205
        color: Theme.backgroundColor
         
        Column {
          anchors {
              top: parent.top
              topMargin: backHomeMenu.spacing
              right: parent.right
              rightMargin: backHomeMenu.spacing
          }
          spacing: backHomeMenu.spacing

          PrimaryButton {
              id: mainMenuButton
              icon.source: cameraIcon("home")
              onClicked: homeClicked()
          }

          PrimaryButton {
              id: exitMenuButton
              icon.source: cameraIcon("close")
              onClicked: exitClicked()
          }
        }
    }
}
