import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    signal done
    ColumnLayout {
        anchors.fill: parent
        PageIndicator {
            id: pi
            Layout.alignment: Qt.AlignHCenter
            // Layout.fillWidth: true
            count: sv.count
            currentIndex: sv.currentIndex
            delegate: Pane {
                Label {
                    // Material.foreground: index == pi.currentIndex ? Material.Blue : Material.Red
                    // required property real index
                    text: index
                    font {
                        pixelSize: index == pi.currentIndex ? 30 : 10
                    }
                }
            }
        }
        SwipeView {
            id: sv
            Layout.fillWidth: true
            Layout.fillHeight: true
            Personal_SignUp {
                id: personal
                Button {
                    text: "Next"
                    onClicked: sv.incrementCurrentIndex()
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                    }
                }
            }
            Account_SingUp {
                id: account
                Button {
                    text: "Next"
                    onClicked: sv.incrementCurrentIndex()
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                    }
                }
            }
            Student_SignUp {
                id: student
                Button {
                    text: "Done"
                    onClicked: done()
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                    }
                }
            }
        }
    }
}
