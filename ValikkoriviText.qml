import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    property string filtteritexti: "hieno homma"
    height: 50
    width: 1600
    RowLayout{
        id: row1
        Layout.fillWidth: true
        Text{
            text:  filtteritexti
            width: 100
            font.pointSize: 12
        }
        RadioButton{
            id: nappi1
            state: "NOTCHECKED"
            states: [
                State {
                    name: "CHECKED"
                    PropertyChanges {
                        target: nappi1
                        checked: true
                    }
                },
                State{
                    name: "NOTCHECKED"
                    PropertyChanges{
                        target: nappi1
                        checked: false
                    }
                }
            ]

            onClicked:
                if (nappi1.state == "CHECKED"){
                    nappi1.state = "NOTCHECKED"
                }else{
                    nappi1.state = "CHECKED"
                }
        }
        TextField{
            placeholderText: qsTr( "Kirjoita")
            width: 70
        }

    }
}
