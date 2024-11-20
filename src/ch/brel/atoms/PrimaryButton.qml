import QtQuick
import QtQuick.Controls

import ch.brel.theme

Button {
    id: control
    width: 175
    height: 160
    state: "normal"

    icon.height: 48
    icon.width: 48

    display: AbstractButton.IconOnly
    highlighted: false

    background: buttonBackground
    Rectangle {
        id: buttonBackground
        color: Theme.buttonColor
        opacity: enabled ? 1 : 0.3
        radius: 5
        //border.color: "#047eff"
        anchors.fill: parent
    }

    states: [
        State {
            name: "down"
            when: control.down && control.enabled

            PropertyChanges {
                target: buttonBackground
                color: Theme.buttonDownColor
            }
        },
        State {
            name: "selected"
            when: control.checked && !control.down && control.enabled

            PropertyChanges {
                target: buttonBackground
                color: Theme.foregroundColor
            }

            PropertyChanges {
                target: control
                icon.color: Theme.buttonColor
            }
        },
        State {
            name: "disabled"
            when: !control.enabled

            PropertyChanges {
                target: control
                opacity: 0.4
            }
        }
    ]
}
