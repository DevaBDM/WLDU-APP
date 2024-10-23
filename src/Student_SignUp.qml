import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property alias studentID: studentIdTf.text
    property alias department: departmentCb.currentIndex
    ColumnLayout {
        anchors.fill: parent
        ListView {
            id: lv
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            spacing: 10
            header: Pane {
                width: parent.width
                height: 60
                Label {
                    anchors.fill: parent
                    text: "Student information"
                    elide: Label.ElideMiddle
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    fontSizeMode: Label.Fit
                    minimumPixelSize: 1
                    font {
                        pixelSize: 100
                    }
                }
            }
            model: ObjectModel {
                RowLayout {
                    width: lv.width
                    Label {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                        text: "WLDU"
                        verticalAlignment: Label.AlignVCenter
                    }
                    TextField {
                        id: studentIdTf
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        placeholderText: "Student ID"
                    }
                }
                ComboBox {
                    id: departmentCb
                    width: lv.width
                    // placeholderText: "Department"
                    model: ['Electrical and Computer Engineering', 'Mechanical Engineering']
                }
            }
        }
    }
}
