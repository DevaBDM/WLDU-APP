import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 411
    height: 884
    property int path: StandardPaths.AppDataLocation

    Settings {
        id: settings
        property string userName: "Set Username"
        property url imageSource: "../assets/icons/circle-user.svg"
        onUserNameChanged: {
            if (userName == "")
                userName = "Set Username";
        }
        onImageSourceChanged: {
            if (imageSource == "")
                imageSource = "../assets/icons/circle-user.svg";
        }
        property string bio
        property string key
        property string firstName
        property string fatherName
        property real phoneNumber
        property string email
        property string gender
        property real studentId
        property string department
        property bool registered: false

        property alias currentHome: swipeViewHome.currentIndex
    }

    header: ToolBar {
        property alias model: headerModel.model
        ColumnLayout {
            anchors.fill: parent
            Repeater {
                model: ["ወልድያ ዩኒቨርሲቲ", "WOLDIA UNIVERSITY"]
                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Label.AlignHCenter
                    verticalAlignment: Label.AlignVCenter

                    text: modelData
                    fontSizeMode: Label.VerticalFit
                    minimumPixelSize: 1
                    font {
                        pixelSize: 100
                        bold: true
                    }
                    elide: Label.ElideMiddle
                }
            }
        }
        RowLayout {
            id: headerRow
            anchors {
                left: parent.left
                right: parent.right
            }

            ToolButton {
                icon.source: "../assets/icons/back.svg"
                leftPadding: 0
                visible: stackView.depth > 1
                onClicked: {
                    headerModel.model = "";
                    stackView.pop();
                }
            }
            Row {
                Layout.fillWidth: true
                Layout.fillHeight: true
                layoutDirection: Qt.RightToLeft
                Repeater {
                    id: headerModel
                    ToolButton {
                        required property var model
                        text: model.name
                        onClicked: model.triggered()
                    }
                }
            }
        }
    }

    footer: ToolBar {
        Row {
            anchors.centerIn: parent
            Repeater {
                model: ListModel {
                    ListElement {
                        name: "Profile"
                        iconSource: "../assets/icons/circle-user.svg"
                    }
                }
                ToolButton {
                    required property real index
                    required property string name
                    required property url iconSource

                    icon.source: iconSource
                    display: index == swipeViewHome.currentIndex ? ToolButton.TextUnderIcon : ToolButton.IconOnly
                    text: name
                    onClicked: swipeViewHome.currentIndex = index
                    opacity: index == swipeViewHome.currentIndex ? 1 : 0.7
                }
            }
        }
    }

    SwipeView {
        id: swipeViewHome
        anchors.fill: parent
        StackView {
            id: stackView
            states: [
                State {
                    name: "full"
                    PropertyChanges {
                        target: root
                        footer.visible: false
                    }
                }
            ]
            state: !!currentItem && currentItem.full ? "full" : ""

            Component.onCompleted: if (settings.registered) {
                push(profileInfo);
            } else {
                push(welcome);
            }
        }
    }

    function profilePicLocation() {
        return StandardPaths.locate(root.path, "ProfilePicCutted.png");
    }

    Component {
        id: profileInfo
        Item {
            property bool full: false
            Profile {
                anchors {
                    fill: parent
                    margins: 20
                }
                model: ObjectModel {
                    Label {
                        width: ListView.view.width
                        text: "Full name: " + settings.firstName + " " + settings.fatherName
                    }
                    Label {
                        width: ListView.view.width
                        text: "Phone: +251" + settings.phoneNumber
                    }
                    Label {
                        width: ListView.view.width
                        text: "Bio: " + settings.bio
                    }
                }
                userName: settings.userName
                source: settings.imageSource
                ToolButton {
                    icon.source: "../assets/icons/edit-profile.svg"
                    anchors {
                        right: parent.right
                        top: parent.top
                    }
                    onClicked: {
                        stackView.push(editProfile);
                    }
                }
            }
        }
    }

    Component {
        id: editProfile
        Register {
            id: register
            property bool full: true
            imageSource: settings.imageSource
            userName: settings.userName
            bio: settings.bio
            key: settings.key
            firstName: settings.firstName
            fatherName: settings.fatherName
            phoneNumber: settings.phoneNumber
            email: settings.email
            gender: settings.gender
            studentId: settings.studentId
            department: settings.department

            onDone: {
                var tempSource = imageSource;
                settings.imageSource = "";
                settings.imageSource = tempSource;
                settings.userName = userName;
                settings.bio = bio;
                settings.key = key;
                settings.firstName = firstName;
                settings.fatherName = fatherName;
                settings.phoneNumber = phoneNumber;
                settings.email = email;
                settings.gender = gender;
                settings.studentId = studentId;
                settings.department = department;
                root.header.model = "";
                stackView.pop();
            }
        }
    }

    Component {
        id: welcome
        Welcome {
            property bool full: true
            onRegisterClicked: stackView.push(register)
        }
    }

    Component {
        id: register
        Register {
            id: register
            property bool full: true
            onDone: {
                settings.imageSource = "";
                settings.imageSource = imageSource;
                settings.userName = userName;
                settings.bio = bio;
                settings.key = key;
                settings.firstName = firstName;
                settings.fatherName = fatherName;
                settings.phoneNumber = phoneNumber;
                settings.email = email;
                settings.gender = gender;
                settings.studentId = studentId;
                settings.department = department;
                settings.registered = true;
                stackView.clear();
                stackView.push(profileInfo);
            }
        }
    }
}
