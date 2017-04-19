//
//  main.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    // Create a mapping from command/module name to a singleton that creates the
    // object as needed.
    // Need factory function with suitable parameters.
    // Maybe instead of object pointers, we get the singleton pointer and it
    // will transparently access the actual value. Maybe like smart pointer.
    // Looking like smart pointer could handle the case of something failing
    // during construction. Or if passed around, an actual copyable smart pointer?

    // Needed:
    // Input handling.
    // Command handling.
    // History class? Quite simple operation.
    // Helper class for finding all files under a given directory.
    // Class to deal with output, errors, and warnings.
    // Transfer class and derived classes.
    // Format classes and then conversions between them.
    
    // Generally needs a way to pass values to an element. So config command
    // with element name and all strings after that passed as they are? Works
    // as a general piece.
    // Element has to have a way to tell what it expects. For user help output.
    // Maybe via normal message output. Call a help method in element.

    std::cout << "Hello, World!\n";
    return 0;
}

/*
 Komennot ja tarvittavat moduulit
 - Set, varasto, historia
 - Dump, varasto (nimeksi get?)
 - History, historia ja lokitietoja, vain datan luontikomennot.
 - Initialize, varasto
 - List, varasto
 - Delete, varasto (remove?)
 - Configure, historia, mikä tahansa, jopa kaikki
 - Help, mikä tahansa, jopa kaikki
 - Run, varasto, historia/loki, kun ei programia niin pakko tarkistaa ettei
   ole mitään ylimääräistä, tai oletetaan ylimääräisen menevän kutsujalle.
   Ilman programia vaatii siirtotavan määrityksen. Tai infoblokin ja neuvottelu.

 Ennen komentoa voisi olla formaatin ja siirron määritys ja mahdolliset parametrit niille.
 - Silloin formaatti jäisi täysin moduulien ulkopuolelle.
 - Oletus voisi olla tulostus käyttäjälle, tai käytännössä pitänee olla.
 - Siirto voi vaatia parametreja.
 - Joka formaatti voinee olla oma argumentti lyhyyden takia.
 - Jaetun muistin siirto vaatinee blokin nimen. Voisi olla oletusarvo.
   Nimi oltava jotta voi ajaa useamman yhtä aikaa.
 - Jos käytetään sockettia niin se vaatii myös parametrin ja oletuksen samoista syistä.
 - Nyt useimmiten käynnistäjä on joku ohjelma ja komentoriviltä käyttö on 
   erikoistapaus joten ei tallenneta siirtoa eikä formaattia konfiguraatioon.
 - Oletus on yksi syöte komentoriviltä jolloin tulostus stdout/stderr.
 - Transfer ja format annettuna ilman komentoa automaattinen serve.
   Koska pelkkä ohjelman ajo on ikään kuin ajo ilman komentoa jolloin mitään ei tehdä.
 - Kun transfer on annettu, jätetään komentorivi huomiotta.
 - Onko syytä sallia eri formaatti syötteelle ja tulosteelle, mikä olennaisesti 
   on tilanne jossa komentoriviltä esim. dumpataan muuttujan arvo? Set periaatteessa
   tekee tätä kun arvo ei ole tiedosto. Samoin dump on helpompi käyttää. Mutta
   olisiko dump helpompi vain jos se kopioi arvon ulos tiedostoon tai dumppaa 
   sellaisenaan ilman mitään konversiota? Konversio olisi oltava käytettävyyden takia.
 - Jos run osaa dumpata syötteen ohjelmalle ajamatta ohjelmaa niin se kattaa 
   testauksen tarpeet.
 - Run voisi myös dumpata tuloksen niin olisi helpompi testata esim. jaetun 
   muistin käyttöä.

 Konfiguraatiokomento lisätään.
 - Moduulin nimi ensin ja sen jälkeen parametrit merkkijonovektorina moduulille.
 - Moduulin nimi on kiinnitetty ylemmällä tasolla koska moduuleja on rajatusti.
 - Moduuli voi määrittää mitä se ottaa vapaasti.
 - Ainoa rajoitus on, että merkkijonoesitys.
 - Moduulin on osattava kertoa helppitekstissä mitä sisällön on oltava tai onko
   sitä ollenkaan.
 - Kaikkia komentoja vastaa moduuli ja joillekin toiminnoille moduulin pitää
   tukea muita. Voisi avata oven esim. set-komennon syötteiden käsittelylle.
 - Jos komento on moduuli, niin sitten helpin on käsiteltävä myös käyttö.
 - Eli help command on oma helppinsä ja help config on sitten omansa. Help voisi
   päteä vain komentoon ja jos halutaan config help niin se tulostaa kaikkien
   moduulien konfiguraatioiden helpit.
 - Olisiko oltava spesifinen metodi jokaiselle helpille vai pitäisikö olla
   geneerinen joka ottaa tunnisteen? Jälkimmäinen joustavampi.
 - Config komennon on sitten tunnettava kaikki modulit.
 - Moduulilla joko on komento tai ei. Sama pätee konfiguraatioon. Nämä voivat
   olla olemassa toisistaan riippumatta.
 - Kokoelma moduuleista on kätevä helpille ja configille. Muuten riittää antaa
   suoraan kaikki tarvittavat.
 - Kokelman luo ohjelma ja se on kuvaus nimeltä objektille. Ohjelma hoitaa nimeämisen.
 - Jos config ei saa parametreja niin tulostetaan nykyinen konfiguraatio. Jos ei
   ole moduulia niin käydään läpi kaikki.
 - Tulosteet siinä järjestyksessä kuin parametrit. Jos ei annettu, niin joku
   ohjelman sisäinen järjestys.
 - Helppien suhteen lokalisaation tuki jotenkin. Ainakin katsottava kuinka toteutetaan.
 - Moduuli ei lue konfiguraatiota tai muuta kuin vasta tarvittaessa.
 - Syötteiden käsittely vaatinee jonkin apuluokan.
 - Ohjelma käynnistettäessä serve-komento on erikoistapaus koska se alkaa lukemaan
   muita komentoja ja sen läsnäolo määrittää mitä luokkaa käytetään viestien
   välitykseen. Jos ei serve-komentoa niin stdout/stderr tulostusobjekti, muutoin
   pitää tarkistaa formaatti ja siirtotapa.
 - Serve on sitten käsiteltävä eräänlaisena erikoistapauksena joka käynnistää
   silmukan. Tosin jos ei ole serveä niin sitten annettu komentorivi olisi ainoa
   syöte silmukalle.
 - Helppiteksti voisi olla jotain joka vaatii rajallisen alustuksen. Eli olisiko
   parametrien luku oma luokka yhdessä objektin kanssa vai olisiko objektin
   toimittava sisäisesti laiskasti jotta syötteet käsitellään sisäisesti mutta
   helpin saa vaikka konfiguraatio rikki.
 - Jos konfiguraatio on laiskasti luettu niin se riittää siihen, että helppi
   saadaan ulos tai syötteet prosessoitua ilman, että konfiguraatio rikkonaisuus
   häiritsee.

*/
