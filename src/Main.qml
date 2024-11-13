import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls
import WLDU
import com.user.db
import "Schedule"
import "Register"
import "Home"
import "User"

ApplicationWindow {
    id: root
    visible: true
    width: 411
    height: 884
    property int path: StandardPaths.AppDataLocation

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
                icon.source: "qrc:/qt/qml/WLDU/assets/icons/back.svg"
                leftPadding: 0
                visible: stackViewHome.depth > 1
                onClicked: {
                    headerModel.model = "";
                    stackViewHome.pop();
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
            ToolButton {
                id: downloadMangerTB
                icon.source: "qrc:/qt/qml/WLDU/assets/icons/download.svg"
                leftPadding: 0
                onClicked: {
                    stackViewHome.push(downloadManger);
                }
            }
        }
    }

    StackView {
        id: stackViewHome
        anchors.fill: parent
        Component.onCompleted: if (User.registered) {
            push(swipeViewHome);
        } else {
            push(welcome);
        }
    }

    Component {
        id: welcome
        Welcome {
            property bool full: true
            Timer {
                id: timer
                interval: 3 * 1000
                repeat: true
                running: !User.registerDB && !!User.RegisterCache && !User.RegisterCache.progress
                onTriggered: {
                    if (!!User.RegisterCache)
                        User.RegisterCache.reFetch();
                }
            }
            info: !!User.RegisterCache ? User.RegisterCache.networkMessage(User.RegisterCache.networkStatus) : "Register"
            enabled: !!User.RegisterCache ? !User.RegisterCache.progress : true
            onRegisterClicked: {
                if (!!User.registerDB) {
                    stackViewHome.push(register);
                } else if (!!User.RegisterCache)
                    User.RegisterCache.reFetch();
            }
        }
    }

    Component {
        id: register
        Register {
            id: register
            property bool full: true
            onDone: {
                stackViewHome.clear();
                stackViewHome.push(swipeViewHome);
            }
        }
    }

    Component {
        id: swipeViewHome
        Home {}
    }

    Component {
        id: downloadManger
        DownloadManger {
            id: dm
            StackView.onRemoved: downloadMangerTB.visible = true
            StackView.onActivating: downloadMangerTB.visible = false
        }
    }
}
