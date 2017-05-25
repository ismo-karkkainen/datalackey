//
//  Storage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include "History.hpp"

class Storage {
    // Make id number.
    // Convert number to char string and split to directories.
    // Get unique file name.
    // Save data when changed.
public:
    Storage(); // Take directory. Find existing storage. Lock.
    ~Storage(); // Save upon destruction. Unlock.
    // Check if valid.
    // Configuration location.
    // Create.
    // Delete value.
    // Store value.
    // Append to value.
    // Clean.
};

#endif /* Storage_hpp */

/*
 Varasto tarvitaan aina tai ainakin hakemiston tarkistus joten varasto pitää 
 alustaa sen verran mutta datan lataus voisi olla vain tarvittaessa.
 - Varaston pitäisi pitää huolta, että vain yksi prosessi pääsee käsittelemään.
 - Jos varasto pitää luoda muusta syystä kuin helpin takia niin lukitus on hoidettava.
 - Onko varastolla joku konfiguraatio jonka luonti vaaditaan jos oikeasti tehdään jotain?
 - Se olisi sopiva paikka lukitsemiselle.

 - Syötteen lukeminen vaatii varaston ja jos ei olla alustettu varastoa niiin
   se ei voi varastoida mitään.
 - Eli varaston olisi oltava jotain jolle osoitetaan paikka minne tallentaa
   tiedostot. Siihen asti varastoidaan muistiin.
 - Koska käynnistetään aliprosessina niin voisiko silti olettaa nykyisen
   hakemiston alta löytyvän varaton ellei toisin osoiteta. Ja voiko vaihtaa
   kesken kaiken tai ennen kuin dataan on koskettu?
 - Olisiko varaston haku aina eksplisiittinen käsky? Eli käytetään muistia
   kunnes toisin käsketään? Sen jälkeen voi käyttää levyä.
 - Tällöin levy olisi vain muistin jatke. Eli jos joku raja täyttyy niin
   käytetään levyä jotta saadaan muistia vapautettua.
 - Voisi olla ehdotus suorittaa lataus etukäteen.

 */
