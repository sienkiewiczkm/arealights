// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once
#include <stdexcept>

namespace fw
{

class NotImplementedException:
    public std::logic_error
{
public:
    NotImplementedException():
        std::logic_error("Function not yet implemented")
    {
    }
};

}
