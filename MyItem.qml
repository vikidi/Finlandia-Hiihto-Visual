import QtQuick 2.0
import QtQuick.Controls 2.0

ListView {
    spacing: 10
    Row{
        id: row1
        Text{
            text: "Filtteri"
            width: 100
        }

        RadioButton{
            id: nappi1
            text: "Valitse filtteri"
        }
        TextField{
            text: "Anna vaikka mini"
            width: 70
        }
        TextField{
            text: "Max"
            width: 70
        }
    }
    Row{
        id: row2
        anchors.top: row1.bottom
        Text{
            text: "Filtteri"
            width: 100
        }

        RadioButton{
            id: nappi2
            text: "Valitse filtteri"
        }
        TextField{
            text: "Anna vaikka mini"
            width: 70
        }
        TextField{
            text: "Max"
            width: 70
        }
    }

}
