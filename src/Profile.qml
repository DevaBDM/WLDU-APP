import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property alias source: ppImage.source
    property alias userName: nameLabel.text
    property alias model: lv.model
    property alias delegate: lv.delegate
    ColumnLayout {
        id: col
        anchors.fill: parent
        Item {
            id: user
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 2
            Item {
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                }
                height: lv.contentY < 0 ? parent.height - lv.contentY : parent.height
                state: height > 9 * parent.height / 4 ? "Expand" : ""
                Image {
                    id: ppImage
                    height: parent.height
                    width: height
                    cache: false
                    asynchronous: true
                    TapHandler {
                        onSingleTapped: {}
                    }
                }

                ColumnLayout {
                    id: fullNameColumn
                    anchors {
                        left: ppImage.right
                        right: parent.right
                        bottom: ppImage.bottom
                    }
                    Label {
                        id: nameLabel
                        Layout.fillWidth: true
                        elide: Label.ElideMiddle
                        font {
                            bold: true
                        }
                    }
                }
                states: [
                    State {
                        name: "Expand"
                        AnchorChanges {
                            target: fullNameColumn
                            anchors.bottom: parent.bottom
                            anchors.left: parent.left
                        }
                        AnchorChanges {
                            target: ppImage
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                ]
                transitions: Transition {
                    AnchorAnimation {
                        duration: 200
                    }
                }
                MenuSeparator {
                    height: 20
                    anchors {
                        verticalCenter: parent.bottom
                        left: parent.left
                        right: parent.right
                    }
                }
            }
        }

        ListView {
            id: lv
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 12

            header: Item {
                id: header
                width: parent.width
                height: user.height * 2
            }
        }
    }
}
