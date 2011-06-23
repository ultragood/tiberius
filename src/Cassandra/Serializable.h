#pragma once

#include <vector>

namespace tiberius{
//    template <class T>
    class Serializable{
        virtual std::vector<char> toBytes() = 0;
        virtual void read(std::vector<char> &bytes) = 0;

    };

}
