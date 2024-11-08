import QtQuick
import QtQuick.Controls

Pane {
    id: root
    property alias userName: userNameTf.text
    property alias bio: bioTf.text
    readonly property alias acceptableInput: userNameTf.acceptableInput
    function animate() {
        if (!userName.acceptableInput) {
            invalidInputAnime.start();
            userNameTf.forceActiveFocus();
        }
    }
    Column {
        anchors.fill: parent
        spacing: 10
        TextField {
            id: userNameTf
            width: parent.width
            placeholderText: "Set username"
            validator: RegularExpressionValidator {
                regularExpression: /[\w]{4,10}/
            }
            Keys.onReturnPressed: event => {
                if (!userNameTf.acceptableInput)
                    invalidInputAnime.start();
                event.accepted = false;
            }
            SpringAnimation {
                id: invalidInputAnime
                target: userNameTf
                property: "x"
                spring: 10
                damping: 0.1
                duration: 100
                from: userNameTf.x - 5
                to: userNameTf.x
            }
        }
        TextField {
            id: bioTf
            width: parent.width
            placeholderText: "Set bio"
            validator: RegularExpressionValidator {
                regularExpression: /.{0,150}/
            }
        }
    }
}
