import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.user.db

Pane {
    ColumnLayout {
        anchors.fill: parent
        Label {
            Layout.fillWidth: true
            text: lv.model.RowCount > 0 ? "Downloaded files" : "No Downloaded files"
            horizontalAlignment: Label.AlignHCenter
            verticalAlignment: Label.AlignVCenter
        }
        RowLayout {
            Label {
                text: "Search"
            }
            TextField {
                Layout.fillWidth: true
                placeholderText: "Enter name of file to search"
                onTextChanged: if (text === "") {
                    User.Schedule.FilesModel.DownloadManger.search('%');
                    return;
                } else {
                    User.Schedule.FilesModel.DownloadManger.search(text);
                }
            }
        }
        MenuSeparator {
            Layout.fillWidth: true
        }

        ListView {
            id: lv
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: User.Schedule.FilesModel.DownloadManger

            delegate: RowLayout {
                width: ListView.view.width
                property var download: User.Schedule.FilesModel.DownloadManger.download(fileName, hash)
                ItemDelegate {
                    Layout.fillWidth: true
                    text: fileName
                    onClicked: {
                        if (!!parent.download)
                            if (parent.download.downloaded)
                                User.openFile(parent.download.fileLocation);
                            else
                                parent.download.startDownload();
                    }
                    onPressAndHold: fileCacheMenu.open()
                    MenuSeparator {
                        anchors {
                            left: parent.left
                            leftMargin: 20
                            right: parent.right
                            bottom: parent.bottom
                        }
                    }
                }
                ToolButton {
                    icon.source: "qrc:/qt/qml/WLDU/assets/icons/trash.svg"
                    onClicked: lv.model.deleteFile(index)
                }
            }
            displaced: Transition {
                PauseAnimation {
                    duration: 200
                }
                NumberAnimation {
                    properties: "y"
                    duration: 300
                }
            }
            remove: Transition {
                NumberAnimation {
                    properties: "x"
                    to: lv.width
                    duration: 200
                }
            }
            add: Transition {
                NumberAnimation {
                    properties: "y"
                    duration: 400
                }
            }
        }
    }
    Component.onCompleted: User.Schedule.FilesModel.DownloadManger.search('%')
}
