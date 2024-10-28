import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ListView {
    id: root
    property var date: new Date()
    property real currentCol
    header: lvHeader

    populate: Transition {
        NumberAnimation {
            properties: "y"
            duration: 200
        }
    }

    Component {
        id: lvHeader
        ColumnLayout {
            width: parent.width
            GridLayout {
                Layout.fillWidth: true
                columns: 3
                rowSpacing: 0
                Column {
                    Layout.rowSpan: 2
                    spacing: 0
                    Label {
                        id: todayLL
                        text: root.date.toLocaleString(Qt.locale(), "dd MMMM")
                        font {
                            pixelSize: 10
                        }
                    }
                    ToolButton {
                        id: todayTB
                        text: "Today"
                        font {
                            bold: true
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.rowSpan: 2
                    color: "red"
                }

                ToolButton {
                    icon.source: "/qt/qml/WLDU/assets/icons/check.svg"
                    text: "Connected"
                }

                ToolButton {
                    Layout.alignment: Qt.AlignRight
                    icon.source: "/qt/qml/WLDU/assets/icons/question.svg"
                }
            }
            MenuSeparator {}
            RowLayout {
                ToolButton {
                    id: privouseTB
                    icon.source: "/qt/qml/WLDU/assets/icons/angle-left.svg"
                    onClicked: root.currentCol <= 0 ? root.currentCol = 6 : root.currentCol--
                }
                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Label.AlignHCenter
                    text: root.currentCol + " September"
                }
                ToolButton {
                    id: nextTB
                    icon.source: "/qt/qml/WLDU/assets/icons/angle-right.svg"
                    onClicked: root.currentCol >= 6 ? root.currentCol = 0 : root.currentCol++
                }
            }
            ListView {
                id: lv
                Layout.fillWidth: true
                implicitHeight: currentItem.height
                orientation: Qt.Horizontal
                snapMode: ListView.SnapOneItem
                model: 1
                clip: true

                delegate: RowLayout {
                    width: ListView.view.width
                    Repeater {
                        model: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
                        Column {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignHCenter
                            ToolButton {
                                text: modelData + "\n" + index
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked: root.currentCol = index
                                opacity: index == root.currentCol ? 1 : 0.7
                            }
                            Rectangle {
                                implicitWidth: parent.width / 2
                                anchors.horizontalCenter: parent.horizontalCenter
                                visible: index == root.currentCol
                                implicitHeight: 4
                                color: "blue"
                            }
                        }
                    }
                }
            }
        }
    }
}
