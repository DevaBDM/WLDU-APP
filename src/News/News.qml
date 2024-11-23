import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db
import WLDU

ListView {
    Timer {
        id: timer
        interval: (!!User.News.Cache && User.News.Cache.networkStatus === CacheManager.Connected ? 30 * 60 : 4) * 1000
        repeat: true
        running: !!User.News.Cache
        triggeredOnStart: true
        onTriggered: User.News.Cache.reFetch()
    }
    spacing: 10
    reuseItems: true
    model: User.News
    header: ColumnLayout {
        width: parent.width
        RowLayout {
            Layout.fillWidth: true
            ToolButton {
                text: "Clear cache"
                onClicked: User.News.clearCache()
            }
            Item {
                Layout.fillWidth: true
            }
            ToolButton {
                Layout.alignment: Qt.AlignRight
                enabled: {
                    if (!!User.News.Cache) {
                        switch (User.News.Cache.networkStatus) {
                        case CacheManager.Connected:
                        case CacheManager.Error:
                        case CacheManager.Waiting:
                            return true;
                        default:
                            return false;
                        }
                    } else
                        return true;
                }

                icon.source: {
                    if (!!User.News.Cache) {
                        switch (User.News.Cache.networkStatus) {
                        case CacheManager.Connected:
                            "/qt/qml/WLDU/assets/icons/circle-check.svg";
                            break;
                        case CacheManager.Error:
                            "/qt/qml/WLDU/assets/icons/circle-exclamation.svg";
                            break;
                        default:
                            "/qt/qml/WLDU/assets/icons/arrow-rotate.svg";
                        }
                    } else
                        "/qt/qml/WLDU/assets/icons/arrow-rotate.svg";
                }
                text: !!User.News.Cache ? User.News.Cache.networkMessage(User.News.Cache.networkStatus) : ""
                onClicked: {
                    if (!!User.News.Cache)
                        User.News.Cache.reFetch();
                }
            }
        }
    }
    delegate: Pane {
        ListView.onPooled: {
            imageViewer.source = "";
        }
        width: ListView.view.width
        ColumnLayout {
            anchors.fill: parent
            Image {
                id: imageViewer
                Layout.preferredHeight: 300
                Layout.fillWidth: true
                Layout.fillHeight: true
                fillMode: Image.PreserveAspectFit
                visible: source != ""
                source: ""
                cache: true
                TapHandler {
                    onDoubleTapped: {
                        User.openFile(imageViewer.source);
                    }
                }
            }
            ListView {
                id: lv
                clip: true
                spacing: 4
                Layout.preferredHeight: 100
                Layout.fillWidth: true
                Layout.fillHeight: true
                orientation: Qt.Horizontal
                model: User.News.hashList(newsID)
                visible: model.rowCount() > 0
                currentIndex: 0
                delegate: Image {
                    height: ListView.view.height
                    width: height
                    property var downloader: User.News.downloader("", display)
                    source: downloader.downloaded ? "file:/" + downloader.fileLocation : "qrc:/qt/qml/WLDU/assets/icons/downloadImage.jpg"
                    TapHandler {
                        onSingleTapped: {
                            if (downloader.downloaded)
                                imageViewer.source = parent.source;
                            else
                                downloader.startDownload();
                        }
                    }
                }
            }
            Label {
                Layout.fillWidth: true
                text: title
                wrapMode: Label.Wrap
                textFormat: Label.MarkdownText
                onLinkActivated: link => {
                    Qt.openUrlExternally(link);
                }
                font {
                    bold: true
                }
            }
            Label {
                Layout.fillWidth: true
                text: header
                wrapMode: Label.Wrap
                textFormat: Label.MarkdownText
                onLinkActivated: link => {
                    Qt.openUrlExternally(link);
                }
            }
            Label {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: main
                wrapMode: Label.Wrap
                onLinkActivated: link => {
                    Qt.openUrlExternally(link);
                }
            }
            Label {
                Layout.fillWidth: true
                text: footer
                wrapMode: Label.Wrap
                textFormat: Label.MarkdownText
                onLinkActivated: link => {
                    Qt.openUrlExternally(link);
                }
            }
        }
    }
    ScrollBar.vertical: ScrollBar {}
}
