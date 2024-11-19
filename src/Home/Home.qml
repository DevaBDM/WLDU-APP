import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import com.user.db
import com.AppUpdate
import "../Schedule"
import ".."
import "../News"
import "../Grade"
import "../Register"

Page {
    id: root
    signal push(Component comp)
    signal about
    states: [
        State {
            name: "full"
            PropertyChanges {
                target: root
                footer.visible: false
            }
        }
    ]
    Settings {
        property alias currentIndex: swipeView.currentIndex
    }
    state: !!swipeView.currentItem && swipeView.currentItem.full ? "full" : ""
    SwipeView {
        id: swipeView
        anchors.fill: parent
        HomeTab {
            id: home
            onCalculateGrade: root.push(gradeUI)
        }
        Pane {
            News {
                id: news
                anchors.fill: parent
            }
        }

        Pane {
            Schedule {
                id: schedule
                anchors.fill: parent
                anchors.margins: 5
            }
        }
        Item {
            Profile {
                id: profile
                anchors {
                    fill: parent
                    margins: 20
                }
                model: ObjectModel {
                    Label {
                        width: !!ListView.view ? ListView.view.width : width
                        text: "User info"
                    }
                    Pane {
                        width: !!ListView.view ? ListView.view.width : width
                        ColumnLayout {
                            anchors.fill: parent
                            Repeater {
                                model: User.profileInfo
                                delegate: ItemDelegate {
                                    text: modelData
                                    leftPadding: 10
                                    Layout.fillWidth: true
                                    MenuSeparator {
                                        anchors {
                                            left: parent.left
                                            leftMargin: 10
                                            right: parent.right
                                            verticalCenter: parent.bottom
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Label {
                        width: !!ListView.view ? ListView.view.width : width
                        text: "Settings"
                    }
                    Pane {
                        width: !!ListView.view ? ListView.view.width : width
                        UserSettings {
                            id: userSettings
                            anchors.fill: parent
                        }
                    }
                    ItemDelegate {
                        text: "About WLDU"
                        Layout.fillWidth: true
                        onClicked: root.about()
                    }
                }

                userName: User.userName
                source: User.pp_location
                ToolButton {
                    icon.source: "qrc:/qt/qml/WLDU/assets/icons/edit-profile.svg"
                    anchors {
                        right: parent.right
                        top: parent.top
                    }
                    z: -1
                    onClicked: root.push(profilePictureEdit)
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
                        name: "Home"
                        iconSource: "qrc:/qt/qml/WLDU/assets/icons/home.svg"
                    }
                    ListElement {
                        name: "News"
                        iconSource: "qrc:/qt/qml/WLDU/assets/icons/news.svg"
                    }
                    ListElement {
                        name: "Schedule"
                        iconSource: "qrc:/qt/qml/WLDU/assets/icons/schedule.svg"
                    }
                    ListElement {
                        name: "Profile"
                        iconSource: "qrc:/qt/qml/WLDU/assets/icons/circle-user.svg"
                    }
                }
                ToolButton {
                    required property real index
                    required property string name
                    required property url iconSource

                    icon.source: iconSource
                    display: index == swipeView.currentIndex ? ToolButton.TextUnderIcon : ToolButton.IconOnly
                    text: name
                    onClicked: swipeView.currentIndex = index
                    opacity: index == swipeView.currentIndex ? 1 : 0.7
                }
            }
        }
    }
    Component {
        id: gradeUI
        Grade {
            id: gradeCalculate
        }
    }
    Component {
        id: profilePictureEdit
        ColumnLayout {
            SwipeView {
                id: editSwipe
                currentIndex: indicator.currentIndex
                Layout.fillWidth: true
                Layout.fillHeight: true
                ProfilePictureRegister {
                    id: profilePicture
                    imageSource: profile.source
                }
                AccountRegister {
                    id: accountEdit
                    userName: User.userName
                    bio: User.bio
                    Button {
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                        }
                        text: "Save"
                        onClicked: {
                            User.userName = accountEdit.userName;
                            User.bio = accountEdit.bio;
                        }
                    }
                }
            }
            PageIndicator {
                id: indicator
                // Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                currentIndex: editSwipe.currentIndex
                count: editSwipe.count
                interactive: true
            }
            StackView.onRemoved: {
                profile.source = "";
                User.pp_location = profilePicture.imageSource;
                profile.source = profilePicture.imageSource;
            }
        }
    }
}
