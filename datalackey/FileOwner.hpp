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
#include <set>
#include <mutex>


class FileOwner : public DataOwner {
private:
    bool owner, present, finished, in_error;
    size_t index;
    std::string full;
    mutable int fd;
    size_t length;
    mode_t mode;

    bool make_dirs(const std::string& Root, const std::string& dir);
    std::string make_name();
    bool append(RawData::ConstIterator From, RawData::ConstIterator To);

    // For finding out ununsed small index.
    static std::set<size_t> freed;
    static std::set<size_t> used;
    static std::mutex index_mutex;

public:
    // Opens an existing file listed in catalog, never owns the file.
    FileOwner(const std::string& Root, size_t Index);
    // Opens a new file that needs to be written.
    FileOwner(const std::string& Root, mode_t FileMode);
    ~FileOwner();

    bool Owner() const;
    void Own(bool OwnFile);
    bool Error() const;
    bool Cacheable() const;
    bool Finished() const;
    size_t Index() const { return index; }
    const std::string& FullName() const;

    bool Append(RawData::ConstIterator From, RawData::ConstIterator To);
    void Finish();

    size_t Size() const;
    std::shared_ptr<const RawData> FullData();

    std::shared_ptr<DataReader> Reader(std::shared_ptr<DataOwner>& Owner);
};


#endif /* FileOwner_hpp */
