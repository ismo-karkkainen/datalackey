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
#include "Label.hpp"
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
    virtual ~Storage();

    virtual bool IsValid() const = 0;

    // Return label, format, size in bytes.
    virtual std::vector<std::tuple<Label,std::string,size_t>> List() const = 0;

    virtual void Store(const Label& L, const char *const Format,
        RawData& Value) = 0;
    void Store(const Label& L, const std::string& Format,
        RawData& Value);
    virtual bool Delete(const Label& L) = 0;
    virtual void Clean() = 0;

    // Pre-load a value.
    virtual bool Preload(const Label& L, const char *const Format) = 0;
    bool Preload(const Label& L, const std::string& Format);

    virtual bool IsReady(const Label& L, const char *const Format) = 0;
    bool IsReady(const Label& L, const std::string& Format);

    // Return the value.
    virtual std::shared_ptr<const RawData> Data(const Label& L, const char *const Format) = 0;
    std::shared_ptr<const RawData> Data(const Label& L,
        const std::string& Format);

    // Return the value only if ready, no conversion done or started.
    virtual std::shared_ptr<const RawData> ReadyData(const Label& L, const char *const Format) = 0;
    std::shared_ptr<const RawData> ReadyData(const Label& L,
        const std::string& Format);
};


#endif /* Storage_hpp */

/*
 Varasto tarvitaan aina tai ainakin hakemiston tarkistus joten varasto pitää 
 alustaa sen verran mutta datan lataus voisi olla vain tarvittaessa.
 - Varaston pitäisi pitää huolta, että vain yksi prosessi pääsee käsittelemään.
 */
