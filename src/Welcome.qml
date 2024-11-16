import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "Home"

Item {
    signal registerClicked
    property alias notice: info.text
    property alias enabled: regButton.enabled
    property alias info: regButton.text

    About {
        id: messageDialog
    }

    ColumnLayout {
        anchors.fill: parent
        ToolButton {
            text: "About"
            Layout.alignment: Qt.AlignRight
            onClicked: messageDialog.open()
        }

        Label {
            Layout.fillWidth: true
            text: "Welcome!!!"
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignBottom
            fontSizeMode: Label.Fit
            minimumPixelSize: 0
            font {
                pixelSize: 50
            }
        }
        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            source: '../assets/images/wldu_logo.png'
            fillMode: Image.PreserveAspectFit
        }
        Label {
            id: info
            Layout.fillWidth: true
            horizontalAlignment: Label.AlignHCenter
            font {
                bold: true
            }
        }
        Button {
            id: regButton
            text: "Register"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            onClicked: {
                messageDialog.open();
                registerClicked();
            }
        }
    }
}
