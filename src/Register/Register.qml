import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db

Item {
    id: root
    signal done

    Connections {
        id: registerSignal
        target: User
        function onRegisteredSuccessfully() {
            done();
        }
    }

    property alias program: student.program
    property alias addmission: student.addmission
    property alias department: student.department
    property alias stream: student.stream
    Settings {
        id: settings
        property alias imageSource: profilePic.imageSource
        property alias userName: account.userName
        property alias bio: account.bio
        property alias studentId: student.studentID
        property alias programIndex: student.programIndex
        property alias addmissionIndex: student.addmissionIndex
        property alias yearIndex: student.yearIndex
        property alias semsterIndex: student.semsterIndex
        property alias sectionIndex: student.sectionIndex
        property alias departmentIndex: student.departmentIndex
        property alias streamIndex: student.streamIndex
    }

    states: State {
        name: "full"
        PropertyChanges {
            target: topRegister
            visible: false
        }
        PropertyChanges {
            target: buttonControl
            visible: false
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Label {
            id: topRegister
            Layout.alignment: Qt.AlignHCenter
            text: "Registering..."
        }
        SwipeView {
            id: sv
            Layout.fillWidth: true
            Layout.fillHeight: true
            StudentRegister {
                id: student
            }
            ProfilePictureRegister {
                id: profilePic
                onFull: {
                    root.state = root.state === "full" ? "" : "full";
                }
            }
            AccountRegister {
                id: account
            }
        }
        Button {
            id: buttonControl
            states: State {
                name: "Done"
                PropertyChanges {
                    target: buttonControl
                    text: !!User.UserCache ? User.UserCache.networkMessage(User.UserCache.networkStatus) : "Done"
                    enabled: !!User.UserCache ? !User.UserCache.progress : true
                    onClicked: {
                        User.registerNew(settings.userName, settings.imageSource, settings.bio, settings.studentId, root.program, root.addmission, settings.yearIndex + 1, settings.semsterIndex + 1, settings.sectionIndex + 1, root.department, settings.departmentIndex + 1, root.stream, settings.streamIndex + 1);
                    }
                }
            }
            state: sv.count - 1 == sv.currentIndex ? "Done" : ""
            Layout.alignment: Qt.AlignRight
            text: "Next"
            onClicked: sv.incrementCurrentIndex()
        }
    }
}
