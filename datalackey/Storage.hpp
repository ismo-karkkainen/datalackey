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
#include <memory>
#include <string>
#include <vector>
#include <tuple>


// Base class for Storage classes.
class Storage {
    // Make id number.
    // Convert number to char string and split to directories.
    // Get unique file name.
    // Save data when changed.

public:
    Storage(); // Take directory. Find existing storage. Lock.
    virtual ~Storage(); // Save upon destruction. Unlock.

    virtual bool IsValid() const = 0;

    // Should return label, format, size.
    virtual std::vector<std::tuple<std::string,std::string,size_t>> List() const = 0;

    virtual void Store(const std::string& Label, const char *const Format,
        RawData& Value) = 0;
    void Store(const std::string& Label, const std::string& Format,
        RawData& Value);
    virtual void Delete(const std::string& Label) = 0;
    virtual void Clean() = 0;

    // Pre-load a value.
    virtual void Preload(const std::string& Label, const char *const Format) = 0;
    void Preload(const std::string& Label, const std::string& Format);

    virtual bool IsReady(const std::string& Label, const char *const Format) = 0;
    bool IsReady(const std::string& Label, const std::string& Format);

    // Return the value.
    virtual std::shared_ptr<const RawData> Data(const std::string& Label, const char *const Format) = 0;
    std::shared_ptr<const RawData> Data(const std::string& Label,
        const std::string& Format);
};


#endif /* Storage_hpp */

/*
 Varasto tarvitaan aina tai ainakin hakemiston tarkistus joten varasto pitää 
 alustaa sen verran mutta datan lataus voisi olla vain tarvittaessa.
 - Varaston pitäisi pitää huolta, että vain yksi prosessi pääsee käsittelemään.

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

 */
