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

            property string title: "Data Fetch"

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
            property string title: "Joku valikko"
            anchors.fill: parent


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

        MyItem {
            property string title: "Valikko"


        }

    }
}
