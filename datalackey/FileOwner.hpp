//
//  FileOwner.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 8.1.18.
//  Copyright © 2018 Ismo Kärkkäinen. All rights reserved.
//

#ifndef FileOwner_hpp
#define FileOwner_hpp

#include "DataOwner.hpp"
#include <string>


class FileOwner : public DataOwner {
private:
    bool owner, present, finished, in_error;
    std::string full, partial;
    mutable int fd;
    size_t length;

    bool make_dirs(const std::string& Root, const std::string& dir);
    bool append(RawData::ConstIterator From, RawData::ConstIterator To);

    //friend FileReader;
    int file_descriptor() const;

public:
    // Opens an existing file.
    FileOwner(const std::string& Root, const std::string& Filename, bool Owned);
    // Opens a new file that needs to be written.
    FileOwner(const std::string& Root, const std::string& Filename);
    ~FileOwner();

    bool Owner() const;
    void Own(bool OwnFile);
    bool Error() const;
    bool Cacheable() const;
    bool Finished() const;

    bool Append(RawData::ConstIterator From, RawData::ConstIterator To);
    void Finish();

    size_t Size() const;
    bool StartRead();
    std::shared_ptr<const RawData> Read(size_t SuggestedBlockSize);
    void FinishRead();
};


#endif /* FileOwner_hpp */
