import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    signal done
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

    Settings {
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

    states: State {
        name: "full"
        PropertyChanges {
            target: pi
            visible: false
        }
        PropertyChanges{
            target:buttonControl
            visible: false
        }
    }

    ColumnLayout {
        anchors.fill: parent
        PageIndicator {
            id: pi
            Layout.alignment: Qt.AlignHCenter
            count: sv.count
            currentIndex: sv.currentIndex
            delegate: Pane {
                Label {
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
            }
            Account_SingUp {
                id: account
                onFull: {
                    root.state = root.state === "full" ? "" : "full";
                }
            }
            Student_SignUp {
                id: student
            }
        }
        Button {
            id: buttonControl
            states: State {
                name: "Done"
                PropertyChanges {
                    target:buttonControl
                    text: "Done"
                    onClicked: done()
                }
            }
            state: sv.count-1 == sv.currentIndex ? "Done" : ""
            Layout.alignment: Qt.AlignRight
            text: "Next"
            onClicked: sv.incrementCurrentIndex()
        }
    }
}
