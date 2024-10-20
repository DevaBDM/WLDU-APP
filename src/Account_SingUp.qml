import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            // Layout.fillHeight: true
            // Layout.maximumHeight: width
            implicitHeight: ppImage.height
            RowLayout {
                // anchors.fill: parent
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
                    // anchors {
                    //     bottom: parent.bottom
                    //     horizontalCenter: parent.horizontalCenter
                    // }
                }
            }
        }
        ListView {
            id: lv
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            spacing: 10
            // header: Item {
            //     width: parent.width
            //     height: 20
            // }
            header: Label {
                width: parent.width
                // height: 50
                text: "Account Information"
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                // fontSizeMode: Label.HorizontalFit
                // minimumPixelSize: 0
                // font {
                //     pixelSize: 50
                // }
            }
            model: ListModel {
                ListElement {
                    label: "Username"
                }
                ListElement {
                    label: "Bio"
                }
            }
            delegate: TextField {
                width: ListView.view.width
                required property string label
                placeholderText: label
            }
        }
    }
}
