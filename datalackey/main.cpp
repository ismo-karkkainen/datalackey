//
//  main.cpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    return 0;
}

/*
 Komennot ja tarvittavat moduulit
 - Dump, varasto (nimeksi get?)
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
 - Kaikkia komentoja vastaa moduuli ja joillekin toiminnoille moduulin pitää
   tukea muita. Voisi avata oven esim. set-komennon syötteiden käsittelylle.
 - Moduulilla joko on komento tai ei. Sama pätee konfiguraatioon. Nämä voivat
   olla olemassa toisistaan riippumatta.
 - Kokelman luo ohjelma ja se on kuvaus nimeltä objektille. Ohjelma hoitaa nimeämisen.
 - Tulosteet siinä järjestyksessä kuin parametrit. Jos ei annettu, niin joku
   ohjelman sisäinen järjestys.
 - Syötteiden käsittely vaatinee jonkin apuluokan.

*/
