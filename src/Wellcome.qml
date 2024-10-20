import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    signal registerClicked
    ColumnLayout {
        anchors.fill: parent
        Label {
            Layout.fillWidth: true
            text: "Welcome!!!"
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignBottom
            fontSizeMode: Text.Fit
            minimumPixelSize: 0
            font {
                pixelSize: 50
            }
        }
        Image {
            Layout.fillHeight: true
            Layout.fillWidth: true
            source: 'file://home/fedora/Pictures/Woldiya-University-logo.png'
            fillMode: Image.PreserveAspectFit
        }
        Button {
            text: "Register"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            onClicked: registerClicked()
        }
    }
}
