import QtQuick
import QtCore
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 411
    height: 884
    property int path: StandardPaths.AppDataLocation

    Settings {
        id: settings
        property string fullName: "Set full Name"
        property url source: "../assets/icons/circle-user.svg"
        onFullNameChanged: {
            if (fullName == "")
                fullName = "Set Name";
        }
        onSourceChanged: {
            if (source == "")
                source = "../assets/icons/circle-user.svg";
        }
    }

    header: ToolBar {
        property alias model: headerModel.model
        ColumnLayout {
            anchors.fill: parent
            Repeater {
                model: ["ወልድያ ዩኒቨርሲቲ", "WOLDIA UNIVERSITY"]
                Label {
                    // Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    horizontalAlignment: Label.AlignHCenter
                    verticalAlignment: Label.AlignVCenter

                    text: modelData
                    fontSizeMode: Label.VerticalFit
                    minimumPixelSize: 1
                    font {
                        pixelSize: 100
                        bold: true
                    }
                    elide: Label.ElideMiddle
                }
            }
        }
        RowLayout {
            id: headerRow
            anchors {
                left: parent.left
                right: parent.right
            }

            ToolButton {
                icon.source: "../assets/icons/back.svg"
                leftPadding: 0
                onClicked: {
                    headerModel.model = "";
                    stackView.pop();
                }
            }
            Row {
                Layout.fillWidth: true
                Layout.fillHeight: true
                layoutDirection: Qt.RightToLeft
                Repeater {
                    id: headerModel
                    ToolButton {
                        required property var model
                        text: model.name
                        onClicked: model.triggered()
                    }
                }
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: welcome
    }

    function profilePicLocation() {
        return StandardPaths.locate(root.path, "ProfilePicCutted.png");
    }

    Component {
        id: profileInfo
        Profile {
            ListModel {
                id: editModel
                ListElement {
                    name: "Done"
                    triggered: function () {
                        settings.source = "";
                        settings.source = root.profilePicLocation();
                        settings.fullName = stackView.currentItem.fullName;
                        root.header.model = "";
                        stackView.pop();
                    }
                }
            }
            fullName: settings.fullName
            source: settings.source
            ToolButton {
                icon.source: "../assets/icons/edit-profile.svg"
                anchors.right: parent.right
                anchors.top: parent.top
                onClicked: {
                    root.header.model = editModel;
                    stackView.push(editProfile);
                }
            }
        }
    }

    Component {
        id: editProfile
        EditProfile {
            source: settings.source
            fullName: settings.fullName
            onSelected: selectedFile => {
                root.header.visible = false;
                stackView.push(cutPicture, {
                    "loadSource": selectedFile,
                    "savePath": StandardPaths.writableLocation(root.path)
                });
            }
        }
    }

    Component {
        id: cutPicture
        CutProfilePic {
            source: StandardPaths.locate(root.path, "ProfilePic.png")
            onSaved: fileName => {
                settings.source = "";
                settings.source = StandardPaths.locate(root.path, fileName);
                root.header.visible = true;
                stackView.pop();
            }
        }
    }

    Component {
        id: welcome
        Welcome {
            onRegisterClicked: stackView.push(register)
        }
    }
    Component {
        id: register
        Register {
            onDone: stackView.replace(this, profileInfo)
        }
    }
}
