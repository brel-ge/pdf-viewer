import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import ch.brel
import ch.brel.theme
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

    Rectangle {
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: parent.width - 205
        color: Theme.backgroundColor

        ListView {
            id: pageList
            anchors.fill: parent
            model: pdfModel
            spacing: 5
            clip: true

            onWidthChanged: {
                pdfModel.pageWidth = width;
            }

            // Efficient rendering with delegates
            delegate: Item {
                width: ListView.view.width
                height: pdfModel.pageHeight

                Image {
                    id: image
                    width: parent.width
                    source: "image://pdf/" + pageNumber
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                    cache: true
                }
            }

            // Smooth scrolling and efficient loading
            ScrollBar.vertical: ScrollBar {

                contentItem: Item {
                    implicitWidth: 30
                    implicitHeight: 60
                    Rectangle {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            horizontalCenter: parent.horizontalCenter
                        }

                        width: 15
                        height: 60
                        color: Theme.backgroundColor
                        radius: 5
                    }
                }
            }

            // Caching for performance
            //cacheBuffer: height * 2
        }
    }

    BackHomeMenu {
        id: backHomeMenu
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        onHomeClicked: {
            App.exit();
        }
    }
}
