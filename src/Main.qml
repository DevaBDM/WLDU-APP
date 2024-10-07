import QtQuick.Controls
import QtCore

ApplicationWindow {
    visible: true

    Profile {
        anchors.fill: parent
        source: StandardPaths.locate(StandardPaths.PicturesLocation, "ProfilePic.png")
        fullName: "Dawit Berhan"
    }
}
