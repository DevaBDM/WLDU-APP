import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property alias imageSource: ppImage.source
    property alias userName: userNameTf.text
    property alias bio: bioTf.text
    property alias key: keyTf.text
    ListView {
        id: lv
        anchors.fill: parent
        clip: true
        spacing: 10
        header: Pane {
            width: parent.width
            height: 60
            Label {
                anchors.fill: parent
                text: "Account information"
                elide: Label.ElideMiddle
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                fontSizeMode: Label.Fit
                minimumPixelSize: 1
                font {
                    pixelSize: 100
                }
            }
        }
        model: ObjectModel {
            Item {
                width: lv.width
                implicitHeight: ppImage.height
                RowLayout {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    Image {
                        id: ppImage
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredWidth: 2
                        Layout.maximumHeight: width
                        fillMode: Image.PreserveAspectFit
                        cache: false
                        asynchronous: true
                        source: "../assets/icons/circle-user.svg"
                    }
                    ToolButton {
                        text: "Set Profile Picture > "
                        Layout.alignment: Qt.AlignBottom
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1
                    }
                }
            }
            TextField {
                id: userNameTf
                width: lv.width
                placeholderText: "Enter your Username(required)"
            }
            TextField {
                id: bioTf
                width: lv.width
                placeholderText: "Enter your Bio(Optional)"
            }
            TextField {
                id: keyTf
                width: lv.width
                placeholderText: "Enter your password"
            }
        }
    }
}
