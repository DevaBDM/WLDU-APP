import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    ObjectModel {
        id: objModel
        RowLayout {
            width: lv.width
            Label {
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                text: "WLDU"
                verticalAlignment: Label.AlignVCenter
            }
            TextField {
                Layout.fillHeight: true
                Layout.fillWidth: true
                placeholderText: "Student ID"
            }
        }
        TextField {
            width: lv.width
            placeholderText: "Department"
        }
    }
    ColumnLayout {
        anchors.fill: parent
        ListView {
            id: lv
            Layout.fillHeight: true
            Layout.fillWidth: true
            clip: true
            spacing: 5
            header: Label {
                width: parent.width
                // height: 50
                text: "Student information"
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                // fontSizeMode: Label.HorizontalFit
                // minimumPixelSize: 0
                // font {
                //     pixelSize: 50
                // }
            }
            model: objModel
        }
    }
}
