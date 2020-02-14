import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle{
    color: "#068d9d"
    Column{
        id: sarake
        Layout.fillWidth: true
        anchors.centerIn: parent.Center
        Repeater{
            model: 10
            Valikkorivi{
                filtteritexti: "UUUS teksti" + index
            }

        }
    }
    Button{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Lisää hakukriteerit"

    }
}

