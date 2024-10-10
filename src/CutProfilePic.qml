import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Page {
    id: root
    property alias source: moveImage.source
    property url loadSource
    property string savePath
    signal saved(string fileName)
    footer: RowLayout {
        Item {
            Layout.fillWidth: true
        }
        Button {
            text: "Save"
            onClicked: {
                canvas.loadImage(root.loadSource);
            }
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        clip: true
        // Pt = width/2,height/2
        contentWidth: moveImage.width + width
        contentHeight: moveImage.height + height
        PinchHandler {
            id: pinch
            target: null
            rotationAxis.enabled: false
            // xAxis.enabled: false
            // yAxis.enabled: false
        }

        Image {
            id: moveImage
            width: sourceSize.width * pinch.persistentScale
            height: sourceSize.height * pinch.persistentScale
            source: "file://home/fedora/Pictures/woldia_w_doar.jpg"
            anchors.centerIn: parent
            asynchronous: true
            cache: false
        }
    }

    Canvas {
        id: canvasShade
        anchors.fill: parent
        property real diameter: width > height ? height : width
        onPaint: {
            var ctx = getContext('2d');
            ctx.save();
            ctx.beginPath();
            ctx.fillStyle = Qt.rgba(0, 0, 0, 0.7);
            ctx.rect(0, 0, width, height);
            // Pc = ((width-diameter)*0.5,(height-diameter)*0.5)
            ctx.arc(width / 2, height / 2, diameter / 2, 0, 360);
            ctx.fill();
            ctx.restore();
        }
    }

    Canvas {
        id: canvas
        states: State {
            name: "saving"
            PropertyChanges {
                target: canvas
                width: canvasShade.diameter
                height: width
            }
        }
        anchors.centerIn: canvasShade
        property bool saving: false

        onPaint: {
            var ctx = getContext('2d');
            ctx.save();
            ctx.beginPath();
            ctx.fillStyle = "red";
            ctx.arc(width / 2, height / 2, canvasShade.diameter / 2, 0, 360);
            ctx.clip();
            ctx.drawImage(root.loadSource, flickable.width / 2 - flickable.contentX - x, flickable.height / 2 - flickable.contentY - y, moveImage.width, moveImage.height);
            ctx.restore();
        }
        onImageLoaded: {
            savePP();
        }
        function savePP() {
            saving = true;
            state = "saving";
        }
        onPainted: {
            if (saving) {
                saving = false;
                if (root.savePath.startsWith("file:///"))
                    root.savePath = root.savePath.slice(7);
                save(root.savePath + "/ProfilePicCutted.png");
                state = "";
                root.saved("ProfilePicCutted.png");
            }
        }
    }
}
