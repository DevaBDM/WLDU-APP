import QtQuick

Rectangle {
    anchors.fill: parent
    property alias text: tracerTxt.text
    color: "gray"
    opacity: 0.5
    border.color: "red"
    border.width: 3
    Text {
        id: tracerTxt
        anchors.centerIn: parent
        text: "TTT"
    }
}
