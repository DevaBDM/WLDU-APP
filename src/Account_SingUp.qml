import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Dialogs as D
import QtQuick.Controls
import WLDU

Item {
    id: root
    property alias imageSource: ppImage.source
    property alias userName: userNameTf.text
    property alias bio: bioTf.text
    property alias key: keyTf.text
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
            header: Pane {
                width: parent.width
                height: 60
                Label {
                    anchors.fill: parent
                    text: "Account information"
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
                Item {
                    width: lv.width
                    implicitHeight: ppImage.height
                    RowLayout {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        Image {
                            id: ppImage
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredWidth: 2
                            Layout.maximumHeight: width
                            fillMode: Image.PreserveAspectFit
                            cache: false
                            asynchronous: true
                        }
                        Button {
                            text: "Choose picture"
                            Layout.alignment: Qt.AlignBottom
                            Layout.fillWidth: true
                            Layout.preferredWidth: 1
                            onClicked: fileDialog.open()
                        }
                    }
                }
                TextField {
                    id: userNameTf
                    width: lv.width
                    placeholderText: "Enter your Username(required)"
                }
                TextField {
                    id: bioTf
                    width: lv.width
                    placeholderText: "Enter your Bio(Optional)"
                }
                TextField {
                    id: keyTf
                    width: lv.width
                    placeholderText: "Enter your password"
                }
            }
        }
    }
}
