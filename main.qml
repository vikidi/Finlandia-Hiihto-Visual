import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

import cppbackend 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Testiqmlui {
        id: tabs
        width: 640; height: 480

        Rectangle {
            property string title: "Red"
            anchors.fill: parent
            color: "#e3e3e3"

            BackEnd {
                id: backend
            }

            TextField {
                text: backend.userName
                placeholderText: qsTr("User name")
                anchors.centerIn: parent

                onTextChanged: backend.userName = text
            }
            ProgressBar {
                anchors.fill: parent
                value: backend.progress
                from: 0
                to: 100
            }

        }

        Rectangle {
            property string title: "Green"
            anchors.fill: parent
            color: "#e3e3e3"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#7fff7f"
                Text {
                    width: parent.width - 20
                    anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                    text: "Flower stems are green"
                    font.pixelSize: 20
                    wrapMode: Text.WordWrap
                }
            }
        }

        Rectangle {
            property string title: "Blue"
            anchors.fill: parent; color: "#e3e3e3"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#7f7fff"
                Text {
                    width: parent.width - 20
                    anchors.centerIn: parent; horizontalAlignment: Qt.AlignHCenter
                    text: "Violets are blue"
                    font.pixelSize: 20
                    wrapMode: Text.WordWrap
                }
            }
        }

    }
}
