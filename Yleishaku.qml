import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Rectangle{
    color: "#80ded9"
    property int leveys: 200
    property int korkeus: 50
    Text{
        id: ohjeteksti
        text: "Lisäa kisoja/matkoja vertailtavaksi valitsemalla arvot ja pinamalla 'lisää matka'"
        font.pixelSize: 24
    }

    GridLayout{
        id: sarake
        columns: 3
        anchors.centerIn: parent.Center
        anchors.top: ohjeteksti.bottom
        anchors.topMargin: 50
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

//------------suoritusaika----------------//
        Text{
            id: suoritusaika
            text: "Suoritusaika/aikaväl"
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

//------------nimi------------------------//
        Text{
            id: nimi
            text: "Urheilijan nimi"
            width: leveys; height: korkeus
        }
        Radionappi{
            width: leveys; height: korkeus

        }
        TextField{
            placeholderText: qsTr( "Kirjoita")
            width: leveys; height: korkeus
        }

//---------sijoitus--------------------//

        Text{
            id: sijoitus
            text: "Sijoitus/sijoitusväli "
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

//-----------sukupuoli------------------//
        Text{
            id: sex
            text: "Sukupuoli"
            width: leveys; height: korkeus
        }
        Radionappi{
            width: leveys; height: korkeus

        }
        TextField{
            placeholderText: qsTr( "Kirjoita")
            width: leveys; height: korkeus
        }

//------------kotimaa-----------------//
        Text{
            id: kotimaa
            text: "Uhreilijan kotimaa"
            width: leveys; height: korkeus
        }
        Radionappi{
            width: leveys; height: korkeus

        }
        TextField{
            placeholderText: qsTr( "Kirjoita")
            width: leveys; height: korkeus
        }

    }
    Button{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Lisää hakukriteerit"
    }
}
