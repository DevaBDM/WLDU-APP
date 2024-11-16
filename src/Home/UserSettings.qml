import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import com.user.db
import com.AppUpdate

ColumnLayout {
    id: root
    property alias url: uriTF.text
    Settings {
        property alias uri: uriTF.text
    }
    signal resetURI(string uri)
    ToolButton {
        Layout.fillWidth: true
        contentItem: RowLayout {
            Label {
                Layout.fillWidth: true
                text: "Check for Update!!"
                verticalAlignment: Qt.AlignVCenter
                elide: Label.ElideRight
            }
            Label {
                text: "Version " + AppUpdate.version
                Layout.alignment: Qt.AlignRight
            }
        }
        onClicked: AppUpdate.fetch()
    }
    RowLayout {
        Layout.fillWidth: true
        ToolButton {
            text: "Local server"
            onClicked: {
                if (uriTF.text === "") {
                    User.setHost("http://10.42.0.1/WLDU");
                    AppUpdate.fetch("http://10.42.0.1/WLDU");
                } else {
                    User.setHost(uriTF.text);
                    AppUpdate.fetch(uriTF.text);
                }
            }
        }
        TextField {
            id: uriTF
            Layout.fillWidth: true
            onAccepted: root.resetURI(text)
        }
    }
}
