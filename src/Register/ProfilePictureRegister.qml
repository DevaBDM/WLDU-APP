import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Dialogs as D
import QtQuick.Controls
import WLDU

Item {
    id: root
    property alias imageSource: ppImage.source
    signal full

    CopyFile {
        id: copyFile
        onFinished: url => {
            full();
            sv.push(cutPicture, {
                "loadSource": fileDialog.selectedFile,
                "savePath": StandardPaths.writableLocation(StandardPaths.AppDataLocation)
            });
        }
    }

    Component {
        id: cutPicture
        CutProfilePic {
            source: StandardPaths.locate(StandardPaths.AppDataLocation, "ProfilePic.png")
            onSaved: fileName => {
                root.imageSource = "";
                root.imageSource = StandardPaths.locate(StandardPaths.AppDataLocation, fileName);
                // root.header.visible = true;
                full();
                sv.pop();
            }
        }
    }

    D.FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
        onAccepted: {
            copyFile.save(selectedFile);
        }
    }
    StackView {
        id: sv
        anchors.fill: parent
        initialItem: ListView {
            id: lv
            clip: true
            spacing: 10
            model: ObjectModel {
                Image {
                    id: ppImage
                    width: lv.width
                    height: width
                    fillMode: Image.PreserveAspectFit
                    cache: false
                    asynchronous: true
                    TapHandler {
                        onSingleTapped: fileDialog.open()
                    }
                }
                Button {
                    text: "Choose picture"
                    width: lv.width
                    onClicked: fileDialog.open()
                }
            }
        }
    }
}
