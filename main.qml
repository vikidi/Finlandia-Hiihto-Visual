import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0


import cppbackend 1.0

Window {
    id: ikkuna
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    BackEnd {
        id: backend
    }


    Testiqmlui {
        id: tabs
        width: 640; height: 480

        Rectangle {

            property string title: "Red"

            color: "#e3e3e3"

            ListView{
                anchors.fill: parent
                anchors.horizontalCenter: parent.Center
                anchors.verticalCenter: parent.Center

                ProgressBar {
                    indeterminate: true
                    id: bar
                    value: backend.progress
                    width: ikkuna.width
                    height: 100
                    from: 0
                    to: 100
                    states: State{
                        name: "datafin"; when: backend.dataDone
                        PropertyChanges {
                            target: bar; indeterminate: false

                        }
                    }

                }


                Text{
                    id: textstring
                    anchors.top: bar.bottom
                    anchors.horizontalCenter: bar.horizontalCenter
                    text: backend.progress + "% of data ready"

                }
            }
        }

        Rectangle {
            property string title: "Green"
            anchors.fill: parent
            color: "#e3e3e3"

            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#7fff7f"
                TextField {
                    text: backend.userName
                    placeholderText: qsTr("User name")
                    anchors.centerIn: parent

                    onTextChanged: backend.userName = text
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
