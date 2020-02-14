import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle{
    id: rect
    anchors.fill: parent
    color: "#068d9d"
    Grid{
        Rectangle{
            color: "#b9d6f2"
            width: 600; height: 800
            ListView{

                Text{
                    id: urheilijaotsikko
                    text: "Urheilija"
                    font.pointSize: 24
                }
                Valikkorivi{
                    id: urheilijaosuus
                    anchors.top: urheilijaotsikko.bottom
                    filtteritexti: "Joku filtteri jee"
                }

            }
        }
        Rectangle{
            color: "#80ded9"
            width: 600
            height: 800
            Text{
                id: kilpailuotsikko
                text: "Kilpailu"
                font.pointSize: 24

            }
            Valikkorivi{
                id: kilpailuosuus
                anchors.top: kilpailuotsikko.bottom
                filtteritexti: "UUUS teksti"

            }
        }
    }
    Button{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Lisää hakukriteerit"

    }
}


//Kilpailu				Urheilija
//Näytä kaikki tulokset				Hiihtäjän nimi	X
//Osallistujien määrä				Keskinopeus
//Sijoitus				Kaikki tiedot
//Paskin				Aika
//Osallistujien lukumäärä
//Joukkue
//Kotimaa	x




