import QtQuick
import QtQuick.Controls
import com.user.db
import "../Schedule"
import ".."

Page {
    id: root
    states: [
        State {
            name: "full"
            PropertyChanges {
                target: root
                footer.visible: false
            }
        }
    ]
    state: !!swipeView.currentItem && swipeView.currentItem.full ? "full" : ""
    SwipeView {
        id: swipeView
        anchors.fill: parent
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
                model: User.profileInfo
                delegate: ItemDelegate {
                    width: ListView.view.width
                    text: modelData
                    leftPadding: 10
                    MenuSeparator {
                        anchors {
                            left: parent.left
                            leftMargin: 10
                            right: parent.right
                            verticalCenter: parent.bottom
                        }
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
                }
            }
        }
        Button {
            text: "One"
        }
        Button {
            text: "Two"
        }
    }

    footer: ToolBar {
        Row {
            anchors.centerIn: parent
            Repeater {
                model: ListModel {
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
}
