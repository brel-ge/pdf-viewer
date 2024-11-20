import QtQuick 2.6
import QtQuick.Window 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import ch.brel
import "../molecules"

Window {
    id: window
    visible: true
    width: 1280
    height: 800
    x: 0
    y: 30
    color: "transparent"
    flags: Qt.FramelessWindowHint

    /*!
    \jsfunction closeAllMenus Closes all open menus.
      */
    function icon(name) {
        if (name === undefined) {
            return "";
        }
        return "qrc:/ch/brel/pdf-viewer-ui/assets/icons/" + name + ".png";
    }

    function actionIcon(name) {
        if (name === undefined) {
            return "";
        }
        return icon("action/" + name);
    }

    function cameraIcon(name) {
        if (name === undefined) {
            return "";
        }
        return icon("camera/" + name);
    }

    ScrollView {
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: parent.width - 205
        clip: true

        PDFViewer {
            id: pdfViewer
            anchors.fill: parent
            source: initialPdfPath
        }
    }

    BackHomeMenu {
        id: backHomeMenu
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
    }

    //ProgressPopup {
    //    id: powerProgressPopup
    //    iconSource: progessIcon("power")
    //    cancelable: false
    //    progress: App.state.powerProgress
    //    solid: true
    //}
    //
    //MouseArea {
    //    anchors.fill: parent
    //    propagateComposedEvents: true
    //
    //    onPressed: mouse => {
    //        App.resetStandbyTimer();
    //        if (!window.currentMenuIsMainMenu()) {
    //            if (timer.running) {
    //                timer.restart();
    //            }
    //        }
    //        if (App.state.standbyState) {
    //            mouse.accepted = true;
    //        } else {
    //            mouse.accepted = false;
    //        }
    //    }
    //
    //    onReleased: mouse => {
    //        mouse.accepted = false;
    //    }
    //    onPositionChanged: mouse => {
    //        mouse.accepted = false;
    //    }
    //    onPressAndHold: mouse => {
    //        mouse.accepted = false;
    //    }
    //    onClicked: mouse => {
    //        mouse.accepted = false;
    //    }
    //}
}
