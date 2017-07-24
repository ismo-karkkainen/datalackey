//
//  Storage.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 14.3.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//

#ifndef Storage_hpp
#define Storage_hpp

#include "RawData.hpp"


// Base class for Storage classes.
class Storage {
    // Make id number.
    // Convert number to char string and split to directories.
    // Get unique file name.
    // Save data when changed.
public:
    Storage(); // Take directory. Find existing storage. Lock.
    ~Storage(); // Save upon destruction. Unlock.

    virtual bool IsValid() = 0;
    // There needs to be a way to get a configuration object and Storage is
    // responsible for long-term storage, if any.
    virtual void Configuration() = 0;

    virtual void Delete(const std::string& Label) = 0;
    // Store could return an object to add buffers to, think mem mgmt through.
    // Maybe leave memory management out of it for now.
    // Or the object etc. could be really just handling vectors internally.
    // Format could also be considered.
    virtual void BeginStore(const std::string& Label, const char *const Format) = 0;
    virtual void Store(const std::string& Label, const RawData& DataToAdd) = 0;
    virtual void EndStore(const std::string& Label) = 0;
    virtual void BeginAppend(const std::string& Label, const char *const Format) = 0;
    virtual void Append(const std::string& Label, const RawData& DataToAdd) = 0;
    virtual void EndAppend(const std::string& Label) = 0;
    virtual void Clean() = 0;

    virtual void Preload(const std::string& label) = 0;
    virtual const RawData& Data(const std::string& Label) = 0;
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
   hakemiston alta löytyvän varaston ellei toisin osoiteta. Ja voiko vaihtaa
   kesken kaiken tai ennen kuin dataan on koskettu? Ei voi.
 - Olisiko varaston haku aina eksplisiittinen käsky? Eli käytetään muistia
   kunnes toisin käsketään? Sen jälkeen voi käyttää levyä. Ei ole.
 - Tällöin levy olisi vain muistin jatke. Eli jos joku raja täyttyy niin
   käytetään levyä jotta saadaan muistia vapautettua.
 - Voisi olla ehdotus suorittaa lataus etukäteen.

 */
