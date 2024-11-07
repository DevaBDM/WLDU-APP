import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db

ListView {
    id: root
    property var date: new Date()
    header: lvHeader
    Timer {
        id: timer
        interval: (User.Schedule.networkStatus === User.Schedule.Connected ? 30 * 60 : 4) * 1000
        repeat: true
        running: true
        triggeredOnStart: true
        onTriggered: User.Schedule.fetch()
    }

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
                    text: !!User.Schedule.title ? User.Schedule.title : ""
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
                    text: User.Schedule.startTime + " am - " + User.Schedule.endTime + " pm"
                }
                ToolButton {
                    Layout.fillWidth: true
                    icon.source: User.Schedule.teacherPP
                    text: !!User.Schedule.teacherName ? User.Schedule.teacherName : ""
                }
                Label {
                    Layout.fillWidth: true
                    text: !!User.Schedule.shortNote ? "Description" : "No description !!"
                    elide: Label.ElideRight
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
                Label {
                    Layout.fillWidth: true
                    elide: Label.ElideRight
                    text: !!User.Schedule.shortNote ? User.Schedule.shortNote : ""
                }
                Label {
                    Layout.fillWidth: true
                    text: !!User.Schedule.description ? User.Schedule.description : ""
                    wrapMode: Label.Wrap
                }
            }
        }
    }

    model: User.Schedule
    delegate: lvDelegate

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
                        onClicked: User.Schedule.currentDate()
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
                    enabled: {
                        switch (User.Schedule.networkStatus) {
                        case User.Schedule.Connected:
                        case User.Schedule.Error:
                        case User.Schedule.Waiting:
                            return true;
                        default:
                            return false;
                        }
                    }
                    icon.source: {
                        switch (User.Schedule.networkStatus) {
                        case User.Schedule.Connected:
                            "/qt/qml/WLDU/assets/icons/circle-check.svg";
                            break;
                        case User.Schedule.Error:
                            "/qt/qml/WLDU/assets/icons/circle-exclamation.svg";
                            break;
                        default:
                            "/qt/qml/WLDU/assets/icons/arrow-rotate.svg";
                        }
                    }
                    text: User.Schedule.networkMessage(User.Schedule.networkStatus)
                    onClicked: User.Schedule.fetch()
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
                    onClicked: {
                        User.Schedule.previousDay();
                    }
                }
                Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Label.AlignHCenter
                    text: User.Schedule.date
                }
                ToolButton {
                    id: nextTB
                    icon.source: "/qt/qml/WLDU/assets/icons/angle-right.svg"
                    onClicked: {
                        User.Schedule.nextDay();
                    }
                }
            }
            RowLayout {
                Layout.fillWidth: true
                Repeater {
                    model: ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun']
                    Column {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                        ToolButton {
                            text: modelData // + "\n" + index
                            anchors.horizontalCenter: parent.horizontalCenter
                            onClicked: User.Schedule.setWeekDay(index)
                            opacity: index == User.Schedule.weekDay ? 1 : 0.7
                        }
                        Rectangle {
                            implicitWidth: parent.width / 2
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: index == User.Schedule.weekDay
                            implicitHeight: 4
                            color: "blue"
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
            required property string shortNote
            required property string teacherName
            required property url teacherPP
            required property string description
            required property string type
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
                onClicked: {
                    User.Schedule.currentRow = index;
                    drawerInfo.open();
                }
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
                        Layout.rowSpan: description == "" ? 2 : 3
                        elide: Label.ElideRight
                        text: type
                    }
                    Label {
                        Layout.fillWidth: true
                        elide: Label.ElideRight
                        text: shortNote
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
                        text: description
                    }
                }
            }
        }
    }
}
