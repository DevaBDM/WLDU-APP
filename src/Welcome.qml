import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import "Home"

Item {
    signal registerClicked
    signal settingsClicked
    property alias notice: info.text
    property alias enabled: regButton.enabled
    property alias info: regButton.text

    ColumnLayout {
        anchors.fill: parent
        ToolButton {
            text: "Settings"
            Layout.alignment: Qt.AlignRight
            onClicked: settingsClicked()
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
            source: 'qrc:/qt/qml/WLDU/assets/images/wldu_logo.png'
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
                registerClicked();
            }
        }
    }
}
