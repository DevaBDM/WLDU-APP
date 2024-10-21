import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls

Item {
    signal done
    Settings {
        id: settings
        property alias imageSource: account.imageSource
        property alias userName: account.userName
        property alias bio: account.bio
        property alias key: account.key
        property alias firstName: personal.firstName
        property alias fatherName: personal.fatherName
        property alias phoneNumber: personal.phoneNumber
        property alias email: personal.email
        property alias gender: personal.gender
        property alias studentId: student.studentID
        property alias department: student.department
    }
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
