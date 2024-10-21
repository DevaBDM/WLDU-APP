import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property alias firstName: firstNameTf.text
    property alias fatherName: fatherNameTf.text
    property alias phoneNumber: phoneNumberTf.text
    property alias email: emailTf.text
    property alias gender: genderCb.currentIndex
    ListView {
        id: lv
        anchors.fill: parent
        clip: true
        spacing: 10
        header: Pane {
            width: parent.width
            height: 60
            Label {
                anchors.fill: parent
                text: "Personal information"
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
            TextField {
                id: firstNameTf
                width: lv.width
                placeholderText: "Enter your name(required)"
            }
            TextField {
                id: fatherNameTf
                width: lv.width
                placeholderText: "Enter your Father Name(required)"
            }
            TextField {
                id: phoneNumberTf
                width: lv.width
                placeholderText: "Enter your Phone number(required)"
            }
            TextField {
                id: emailTf
                width: lv.width
                placeholderText: "Enter your email(optional)"
            }
            ComboBox {
                id: genderCb
                width: lv.width
                model: ['M', 'F']
            }
        }
    }
}
