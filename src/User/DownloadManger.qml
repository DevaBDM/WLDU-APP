import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.user.db

Pane {
    ColumnLayout{
                anchors.fill: parent
            Label {
                Layout.fillWidth: true
                text: lv.model.RowCount > 0 ? "Downloaded files" : "No Downloaded files"
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
            }
            MenuSeparator {
                Layout.fillWidth: true
            }

            ListView {
                id: lv
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip:true
                model: User.Schedule.FilesModel.DownloadManger

                delegate: RowLayout {
                    width: ListView.view.width
                    property var download: User.Schedule.FilesModel.DownloadManger.download(fileName, hash)
                    ItemDelegate {
                        Layout.fillWidth: true
                        text: fileName
                        onClicked: {
                            if (!!parent.download)
                            parent.download.startDownload();
                        }
                        onPressAndHold: fileCacheMenu.open()
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
}
