import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import com.user.db
import "../Schedule"
import ".."
import "../News"

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
    function resetURI(uri) {
        User.News.Cache.reFetch(uri);
        User.Schedule.Cache.reFetch(uri);
        User.Schedule.FilesModel.setHost(uri);
    }
    Settings {
        property alias currentIndex: swipeView.currentIndex
        property alias uri: uriTF.text
    }
    state: !!swipeView.currentItem && swipeView.currentItem.full ? "full" : ""
    SwipeView {
        id: swipeView
        anchors.fill: parent
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
                                delegate: ToolButton {
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
                            }
                        }
                    }
                    Label {
                        width: !!ListView.view ? ListView.view.width : width
                        text: "Settings"
                    }
                    Pane {
                        width: !!ListView.view ? ListView.view.width : width
                        ColumnLayout {
                            anchors.fill: parent
                            RowLayout {
                                Layout.fillWidth: true
                                ToolButton {
                                    text: "Local server"
                                    onClicked: {
                                        if (uriTF.text === "") {
                                            root.resetURI("http://10.42.0.1/WLDU");
                                        } else {
                                            root.resetURI(uriTF.text);
                                        }
                                    }
                                }
                                TextField {
                                    id: uriTF
                                    Layout.fillWidth: true
                                    onAccepted: root.resetURI(text)
                                }
                            }
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
                    z: -1
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
}
