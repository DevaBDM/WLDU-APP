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

    Drawer {
        id: drawerInfo
        width: parent.width
        height: parent.height
        edge: Qt.BottomEdge
        ScrollView {
            anchors.fill: parent
            contentWidth: parent.width
            ColumnLayout {
                id: infoPaneLayout
                anchors {
                    fill: parent
                    leftMargin: 20
                    rightMargin: 20
                }
                spacing: 10
                property var currentSchedule: 0
                Label {
                    Layout.fillWidth: true
                    text: root.model.get(parent.currentSchedule).title
                    fontSizeMode: Label.VerticalFit
                    elide: Label.ElideRight
                    font {
                        pixelSize: 50
                        bold: true
                    }
                }
                Label {
                    Layout.fillWidth: true
                    elide: Label.ElideRight
                    text: root.model.get(parent.currentSchedule).startTime + " am - " + root.model.get(parent.currentSchedule).startTime + " pm"
                }
                Label {
                    Layout.fillWidth: true
                    text: root.model.get(parent.currentSchedule).shortNote != "" ? "Description" : "No description !!"
                    elide: Label.ElideRight
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
                Label {
                    Layout.fillWidth: true
                    elide: Label.ElideRight
                    text: root.model.get(parent.currentSchedule).description
                }
                Label {
                    Layout.fillWidth: true
                    text: root.model.get(parent.currentSchedule).note
                    wrapMode: Label.Wrap
                }
                Label {
                    Layout.fillWidth: true
                    elide: Label.ElideRight
                    text: root.model.get(parent.currentSchedule).source != "" ? "Outline" : "No outline available !!"
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
                Flickable {
                    interactive: false
                    Layout.margins: 20
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.preferredHeight: 4
                    Layout.minimumHeight: outlineImage.height / 2
                    Layout.maximumHeight: 500

                    contentWidth: outlineImage.width
                    contentHeight: outlineImage.height
                    clip: true
                    PinchHandler {
                        id: pH
                        target: parent
                        rotationAxis.enabled: false
                        scaleAxis.maximum: 5
                        scaleAxis.minimum: 0.5
                    }
                    Image {
                        id: outlineImage
                        horizontalAlignment: Image.AlignHCenter
                        source: root.model.get(parent.currentSchedule).outlineSource
                    }
                }
                Label {
                    Layout.fillWidth: true
                    text: root.model.get(parent.currentSchedule).attachments != "" ? "Attachments" : "No attachments !!"
                    elide: Label.ElideRight
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
            }
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
            Item {
                Layout.fillWidth: true
                implicitHeight: 7
            }
        }
    }

    Component {
        id: lvDelegate
        RowLayout {
            width: ListView.view.width
            required property real index
            required property string startTime
            required property string endTime
            required property string title
            required property string description
            required property string teacherName
            required property url teacherPP
            required property string note
            required property string type
            required property string outlineSource
            required property real attachments
            required property string status
            required property bool seen

            spacing: 10
            Column {
                Layout.fillHeight: true
                spacing: 10
                Label {
                    text: startTime
                    font {
                        bold: true
                        pixelSize: 20
                    }
                }
                Label {
                    text: endTime
                }
            }
            ColumnLayout {
                Layout.preferredWidth: 1
                Layout.minimumWidth: 10
                Layout.maximumWidth: 20
                Layout.fillHeight: true
                Layout.fillWidth: true
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: width
                    radius: width / 2
                    color: "blue"
                }
                Rectangle {
                    Layout.fillHeight: true
                    Layout.bottomMargin: 5
                    Layout.alignment: Qt.AlignHCenter
                    implicitWidth: 2
                    color: "blue"
                }
            }
            Button {
                Layout.preferredWidth: 150
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                contentItem: GridLayout {
                    columns: 2
                    Label {
                        text: title
                        Layout.fillWidth: true
                        elide: Label.ElideRight
                        font {
                            bold: true
                            pixelSize: 20
                        }
                    }
                    Label {
                        Layout.alignment: Qt.AlignRight
                        Layout.maximumWidth: 60
                        Layout.minimumWidth: 25
                        Layout.fillWidth: true
                        Layout.rowSpan: note == "" ? 2 : 3
                        elide: Label.ElideRight
                        text: type
                    }
                    Label {
                        Layout.fillWidth: true
                        elide: Label.ElideRight
                        text: description
                    }
                    ToolButton {
                        Layout.alignment: Qt.AlignRight
                        Layout.fillWidth: true
                        icon.source: teacherPP
                        text: teacherName
                    }
                    Label {
                        Layout.fillWidth: true
                        elide: Label.ElideRight
                        text: note
                    }
                }
            }
        }
    }
}
