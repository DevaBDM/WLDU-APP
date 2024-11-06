import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.register.db

Pane {
    id: root
    property alias studentID: studentIdTf.text
    property alias programIndex: programCb.currentIndex
    property alias program: programCb.currentText
    property alias addmissionIndex: addmissionCb.currentIndex
    property alias addmission: addmissionCb.currentText
    property alias yearIndex: yearCb.currentIndex
    property alias semsterIndex: semsterCb.currentIndex
    property alias sectionIndex: sectionCb.currentIndex
    property alias departmentIndex: departmentCb.currentIndex
    property alias department: departmentCb.currentText
    property alias streamIndex: streamCb.currentIndex
    property alias stream: streamCb.currentText

    ListView {
        id: lv
        anchors.fill: parent
        spacing: 10
        model: ObjectModel {
            RowLayout {
                width: lv.width
                Label {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignVCenter
                    text: "Student ID"
                    verticalAlignment: Label.AlignVCenter
                }
                TextField {
                    id: studentIdTf
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    placeholderText: "Enter your Student ID without WDU"
                    inputMethodHints: Qt.ImhDigitsOnly | Qt.ImhPreferNumbers
                    validator: RegularExpressionValidator {
                        regularExpression: /[\d]{7}/
                    }
                    Keys.onReturnPressed: event => {
                        if (!studentIdTf.acceptableInput)
                            invalidInputAnime.start();
                        event.accepted = false;
                    }
                    SpringAnimation {
                        id: invalidInputAnime
                        target: studentIdTf
                        property: "x"
                        spring: 10
                        damping: 0.1
                        duration: 100
                        from: studentIdTf.x - 5
                        to: studentIdTf.x
                    }
                }
            }
            ComboBox {
                id: programCb
                width: lv.width
                displayText: currentText + " Program"
                model: Register.program_type()
            }
            ComboBox {
                id: addmissionCb
                width: lv.width
                displayText: currentText + " Admission"
                // model: ['Regular', 'Extension', 'Summer', 'Remedial']
                model: Register.addmission_type()
            }
            ComboBox {
                id: yearCb
                width: lv.width
                displayText: currentText + " Year"
                model: ['1st', '2nd', '3rd', '4th', '6th', '7th', '8th', '9th', '10th', '11th']
            }
            ComboBox {
                id: semsterCb
                width: lv.width
                displayText: currentText + " Semster"
                model: ['1st', '2nd', '3rd']
            }
            ComboBox {
                id: sectionCb
                width: lv.width
                displayText: "Section " + currentText
                model: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25]
            }
            ComboBox {
                id: departmentCb
                width: lv.width
                model: Register.department()
            }
            ComboBox {
                id: streamCb
                width: lv.width
                displayText: currentText + " Stream"
                model: Register.department_stream()
            }
        }
    }
}
