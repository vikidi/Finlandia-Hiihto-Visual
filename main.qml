import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Controls.Styles 1.4
import cppbackend 1.0

Window {
    id: ikkuna
    visible: true
    width: 1200
    height: 800
    title: qsTr("Lujamainen Finlandia-hiihto")
    color: "#809bce"

    BackEnd {
        id: backend
    }


    Testiqmlui {
        id: tabs
        width: 1200; height: 800


        // DATA FETCHING TAB:
        Rectangle {

            property string title: "Data Fetch"

            color: "#0353a4"

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

        // VALIKKO 1
        Rectangle {
            property string title: "Joku valikko"
            anchors.fill: parent


            Rectangle {
                anchors.fill: parent; anchors.margins: 20
                color: "#80ded9"
                TextField {
                    text: backend.userName
                    placeholderText: qsTr("User name")
                    anchors.centerIn: parent

                    onTextChanged: backend.userName = text
                }
            }
        }

        // KISANHAKU JA -LISÄYS TAB
        Yleishaku{
            property string title: "Yleishaku"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 250
            anchors.fill: parent

        }

        // TULOSTEN LISÄSUODATUS
        Esitysvalikko{
            property string title: "Esitysvalinnat"
        }

        // TODO: Hakujen listaus


        // TULOKSET TAB
        ListedResults {
            property string title: "Tulokset"
        }

        // TODO: GRAAFINEN ESITYSTAB

    }
}
