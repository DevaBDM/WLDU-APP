import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
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
                text: "Personal information"
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                // fontSizeMode: Label.HorizontalFit
                // minimumPixelSize: 0
                // font {
                //     pixelSize: 50
                // }
            }
            model: ListModel {
                ListElement {
                    label: "First Name"
                }
                ListElement {
                    label: "Father Name"
                }
                ListElement {
                    label: "Phone Number"
                }
                ListElement {
                    label: "Email"
                }
            }
            delegate: TextField {
                width: ListView.view.width
                placeholderText: label
            }
            footer: ComboBox {
                width: parent.width
                model: ['M', 'F']
            }
        }
    }
}
