import QtQuick
import QtCore
import QtQuick.Dialogs as D
import QtQuick.Controls
import QtQuick.Layouts
import WLDU

ColumnLayout {
    id: root
    property alias fullName: fullNameTf.text
    property alias source: ppImage.source
    signal selected(url file)

    CopyFile {
        id: copyFile
        onFinished: url => {
            root.selected(fileDialog.selectedFile);
        }
    }

    Image {
        id: ppImage
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.maximumHeight: width
        fillMode: Image.PreserveAspectFit
        cache: false
        asynchronous: true
        Button {
            text: "Change Picture"
            anchors {
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            onClicked: fileDialog.open()
        }
    }
    TextField {
        id: fullNameTf
        Layout.fillWidth: true
        placeholderText: "Enter your full name"
    }
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    D.FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        nameFilters: ["Image files (*.png *.jpg *.jpeg)"]
        onAccepted: {
            copyFile.save(selectedFile);
        }
    }
}
