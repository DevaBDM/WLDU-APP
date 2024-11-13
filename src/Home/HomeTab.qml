import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db

Pane {
    ColumnLayout {
        anchors.fill: parent
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Label {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            text: "Courses"
            horizontalAlignment: Qt.AlignHCenter
            font {
                bold: true
                pixelSize: 40
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: User.Schedule.Slip
            orientation: Qt.Horizontal
            spacing: 10
            Layout.maximumHeight: !!currentItem ?  currentItem.height : 0
            Layout.minimumHeight:  !!currentItem ?  currentItem.height : 0
            delegate: Button {
                onClicked: {
                    User.Schedule.Slip.currentRow = index;
                    drawerInfo.open();
                }
                contentItem: ColumnLayout {
                    Label {
                        Layout.fillWidth: true
                        text: title
                        font {
                            bold: true
                            pixelSize: 16
                        }
                    }
                    Label {
                        Layout.fillWidth: true
                        leftPadding: 20
                        text: code
                    }
                    Label {
                        Layout.fillWidth: true
                        text: ECTs + " ECTs"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: LectureHR + " lecture hour"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: tutorHR + " tutor hour"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: lab_practiceHR + " lab practice hour"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: home_studyHR + " home study hour"
                    }
                    Label {
                        Layout.fillWidth: true
                        text: "Pre/Co"
                    }
                    Label {
                        Layout.fillWidth: true
                        leftPadding: 20
                        text: description
                    }
                }
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
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
                Label {
                    Layout.fillWidth: true
                    text: !!User.Schedule.Slip.title ? User.Schedule.Slip.title : ""
                    fontSizeMode: Label.VerticalFit
                    elide: Label.ElideRight
                    font {
                        pixelSize: 50
                        bold: true
                    }
                }
                // ToolButton {
                //     Layout.fillWidth: true
                //     icon.source: User.Schedule.Slip.teacherPP
                //     text: !!User.Schedule.Slip.teacherName ? User.Schedule.teacherName : ""
                // }
                Label {
                    Layout.fillWidth: true
                    text: !!User.Schedule.Slip.shortNote ? "Description" : "No description !!"
                    elide: Label.ElideRight
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
                Label {
                    Layout.fillWidth: true
                    elide: Label.ElideRight
                    text: !!User.Schedule.Slip.shortNote ? User.Schedule.Slip.shortNote : ""
                }
                Label {
                    Layout.fillWidth: true
                    text: !!User.Schedule.Slip.description ? User.Schedule.Slip.description : ""
                    wrapMode: Label.Wrap
                }

                Label {
                    Layout.fillWidth: true
                    text: User.Schedule.FilesModel.rowCount ? "Attachments" : "No attachments !!"
                    elide: Label.ElideRight
                    font {
                        pixelSize: 30
                        bold: true
                    }
                }
                Repeater {
                    model: User.Schedule.FilesModel
                    delegate: ItemDelegate {
                        Layout.fillWidth: true
                        property var download: User.Schedule.FilesModel.downloader(index)
                        enabled: !!download && (!download.downloading && !download.waitting)
                        icon.source: if (!!download) {
                            if (download.downloaded)
                                return "qrc:/qt/qml/WLDU/assets/icons/check.svg";
                            else if (download.downloading || download.waitting)
                                return "qrc:/qt/qml/WLDU/assets/icons/arrow-rotate.svg";
                            else
                                return "qrc:/qt/qml/WLDU/assets/icons/download.svg";
                        } else
                            return ""
                        text: !!download && download.downloading ? download.progress + "% " + fileName : fileName
                        onClicked: {
                            if (!!download)
                                download.startDownload();
                        }
                    }
                }
            }
        }
    }
}
