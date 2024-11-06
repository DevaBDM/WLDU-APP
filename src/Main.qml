import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db
import "Schedule"
import "Register"
import "Home"

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
            onRegisterClicked: stackViewHome.push(register)
        }
    }

    Component {
        id: register
        Register {
            id: register
            property bool full: true
            onDone: {
                stackViewHome.clear();
                stackViewHome.push(busyIndicator);
            }
        }
    }

    Connections {
        target: User
        function onRegisteredSuccessfully() {
            stackViewHome.clear();
            stackViewHome.push(swipeViewHome);
        }
    }

    Component {
        id: swipeViewHome
        Home {}
    }

    Component {
        id: busyIndicator
        BusyIndicator {
            property bool full: true
        }
    }
}
