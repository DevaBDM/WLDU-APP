import QtQuick.Controls

Dialog {
    id: messageDialog
    title: "About"
    anchors.centerIn: parent
    Label {
        anchors.fill: parent
        text: "Welcome to WLDU! This application was created with the goal of providing easy access to the **WOLDIA UNIVERSITY** resource.
            your support and feedback are invaluable to making this app even better.
            \n
            \n# Developer Information:
            \n- Name: **Dawit Berhan**
            \n- Email: dawitberhandm@gmail.com
            \n- Phone: [+251 986621771](tel:+251986621771)
            \n- Telegram Account: [@Deva_B_D_M](https://t.me/Deva_B_D_M)
            \n- Telegram Channel for Update: [WLDU](https://t.me/WLDUAPP)
            \n
            \n# Stay Updated
            \nfor the latest updates, features and announcements, follow Telegram channel.
            \nI'm constantly working to improve your experience and welcome your suggestions or bug reports! "
        wrapMode: Label.Wrap
        textFormat: Label.MarkdownText
        onLinkActivated: link => {
            Qt.openUrlExternally(link);
        }
    }
}
