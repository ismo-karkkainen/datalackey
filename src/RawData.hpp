//
//  RawData.hpp
//  datalackey
//
//  Created by Ismo Kärkkäinen on 18.7.17.
//  Copyright © 2017 Ismo Kärkkäinen. All rights reserved.
//
// Licensed under Universal Permissive License. See License.txt.

#ifndef RawData_hpp
#define RawData_hpp

#include <vector>
#include <cstddef>


class RawData {
private:
    std::vector<char> buffer;
    // Keep track of how much we have used previously for smarter clearing?
public:
    typedef decltype (buffer) Buffer;
    typedef decltype (buffer.begin()) Iterator;
    typedef decltype (buffer.cbegin()) ConstIterator;

    void Clear(bool LikelyUnused = false); // Clears the buffer.
    size_t Size() const { return buffer.size(); }
    bool Empty() const { return buffer.empty(); }
    const char* Raw(size_t Start = 0) const { return &(buffer[Start]); }
    char* Get(size_t Required); // Returns pointer to Required usable bytes.
    void Discard(size_t Unused); // Discard Unused bytes.

    ConstIterator CBegin() const { return buffer.cbegin(); }
    ConstIterator CEnd() const { return buffer.cend(); }
    Iterator Begin() { return buffer.begin(); }
    Iterator End() { return buffer.end(); }

    void Append(ConstIterator From, ConstIterator To);
    void Append(const char Item);
    void Append(const char *const Item);

    void Swap(RawData& RD);
};

#endif /* RawData.hpp */
