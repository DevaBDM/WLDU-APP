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
    model: User.News
    header: ColumnLayout {
        width: parent.width
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
    delegate: Pane {
        width: ListView.view.width
        ColumnLayout {
            anchors.fill: parent
            Label {
                Layout.fillWidth: true
                text: title
                wrapMode: Label.Wrap
                font {
                    bold: true
                }
            }
            Label {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: main
                wrapMode: Label.Wrap
            }
            Label {
                Layout.fillWidth: true
                text: footer
                wrapMode: Label.Wrap
            }
        }
    }
    ScrollBar.vertical: ScrollBar {}
}
