import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0


Rectangle {
//-------------Matka----------------//
            Text{
                id: matka
                text: "Matka (esim P100)"
                width: leveys; height: korkeus
            }
            Radionappi{
                width: leveys; height: korkeus
            }
            TextField{
                placeholderText: qsTr( "Kirjoita")
                width: leveys; height: korkeus
            }

//--------------Vuosi----------------//
            Text{
                id: vuosi
                text: "Vuosi/vuosiväli"
                width: leveys; height: korkeus
            }
            Radionappi{
                width: leveys; height: korkeus

            }
            Row{
                TextField{
                    placeholderText: qsTr( "Min")
                    width: leveys/2; height: korkeus
                }
                TextField{
                    placeholderText: qsTr( "Max")
                    width: leveys/2; height: korkeus
                }
            }


}

//Kilpailu
//Näytä kaikki tulokset
//Osallistujien määrä
//Sijoitus
//Paskin
//Osallistujien lukumäärä
//Joukkue
//Kotimaa
