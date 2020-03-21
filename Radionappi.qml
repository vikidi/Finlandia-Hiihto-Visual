import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
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
}
