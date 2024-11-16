import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import com.user.db

Pane {
    id: root
    ColumnLayout {
        anchors.fill: parent
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: User.Schedule.Slip
            header: Label {
                width: ListView.view.width
                text: "Result = " + User.Schedule.Slip.GradeResult / 1000
                font {
                    pixelSize: 25
                    bold: true
                }
                horizontalAlignment: Qt.AlignHCenter
            }
            delegate: RowLayout {
                width: ListView.view.width
                Label {
                    Layout.fillWidth: true
                    text: title
                    elide: Label.ElideRight
                }
                ComboBox {
                    model: User.Schedule.Slip.LetterGrade
                    currentIndex: gradeIndex
                    onActivated: LetterRow => {
                        User.Schedule.Slip.setGrade(index, LetterRow);
                    }
                }
            }
        }
    }
}
