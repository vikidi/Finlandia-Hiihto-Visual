import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    property string filtteritexti: "hieno homma"
    property int leveys: 1600
    height: 50
    width: 1600
    RowLayout{
        id: row1
        width: leveys
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
            placeholderText: qsTr( "Min")
            width: 20
            height: 10
        }
        TextField{
            placeholderText: qsTr( "Max")
            width: 20
            height: 10
        }
    }
}
