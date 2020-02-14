import QtQuick 2.0

ListView{
    width: 720
    height: 480

    Component {
        id: contactsDelegate

        Rectangle{
            id: wrapper
            width: 720
            height: contactInfo.height
            // Jos haluaisi valita tuloksen ja tehda jtn:
//            color: ListView.isCurrentItem ? "black" : "white"

            // Kaikilla listan itemeilla staattinen vari:
//            color: "white"

            // Vaihtelevat varit:
            color: index % 2 == 0 ? "black" : "white"
            Text {
                id: contactInfo

                text: "year: " + year + " style: " + " time: " + time + " place: "+ place +
                      " place/Men :" + placeMen + " place/women: " + placeWomen + "\n" +
                      "name: " + name + " origin: " + origin + " nationality: " +
                      nationality + " Year of birth: " + birthYr + " team: " + team
//            color: wrapper.ListView.isCurrentItem ? "white" : "black"
//            color: "black"
            color: index % 2 == 0 ? "white" : "black"
            }
        }
    }

    model: ListedModel {}

    /* Esimerkki ListedModelista
    ListModel {
        ListElement {
            year: "2019"
            style: "P100"
            time: "3:22.12"
            place: "1"
            placeMen: "1"
            placeWomen: ""
            name: "Mursu Esa"
            origin: "Karhula"
            nationality: "FI"
            birthYr: "20"
            team: "Karhulan Sudet" }
    }
    */
    delegate: contactsDelegate

}
