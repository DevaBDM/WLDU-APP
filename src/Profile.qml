import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ListView {
    id: lv
    spacing: 4
    clip: true

    property string source
    property string fullName: "Set full Name here"

    headerPositioning: ListView.OverlayHeader
    boundsBehavior: Flickable.DragOverBounds

    readonly property real min: Math.min(lv.width, lv.height) / 3
    readonly property real max: Math.min(lv.width, lv.height)
    property real num: min ? min : 4000

    Binding {
        id: maxBinding
        property bool enabled: false
        when: enabled
        lv.num: lv.max > 0 ? lv.max : 4000
    }

    onDragEnded: {
        if (-contentY > min + 50 && num == min) {
            maxBinding.enabled = true;
            contentY = -num;
        } else if (-contentY < max - 5 && num == max) {
            maxBinding.enabled = false;
            contentY = -num;
        }
    }

    onMovementEnded: {
        if (num == max && contentY < -min) {
            contentY = -num;
        }
    }

    header: Pane {
        width: parent.width
        z: 2
        state: -lv.contentY > lv.min / 2 + lv.max / 2 ? "Expand" : ""

        height: {
            if (lv.dragging) {
                return -lv.contentY > lv.min ? -lv.contentY : lv.min;
            }
            return lv.num;
        }

        Image {
            id: pp
            height: parent.height
            width: height
            cache: false
            source: lv.source
            asynchronous: true
            TapHandler {
                onSingleTapped: {
                    maxBinding.enabled = !maxBinding.enabled;
                    lv.contentY = -lv.num;
                }
            }
        }

        ColumnLayout {
            id: fullNameColumn
            anchors {
                left: pp.right
                right: parent.right
                bottom: pp.bottom
            }
            Label {
                Layout.fillWidth: true
                text: lv.fullName
                elide: Label.ElideMiddle
                font {
                    bold: true
                }
            }
        }
        states: [
            State {
                name: "Expand"
                AnchorChanges {
                    target: fullNameColumn
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                }
                AnchorChanges {
                    target: pp
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        ]
        transitions: Transition {
            AnchorAnimation {
                duration: 200
            }
        }
        MenuSeparator {
            bottomPadding: -2
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }
}
