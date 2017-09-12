// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "StreamUtils.hpp"

namespace fw
{

std::vector<unsigned char> loadStream(std::istream& stream)
{
    if (stream.fail())
    {
        throw std::logic_error("Stream is in fail state.");
    }

    stream.seekg(0, stream.end);
    auto length = stream.tellg();
    stream.seekg(0, stream.beg);

    std::vector<unsigned char> buffer(length);
    stream.read(reinterpret_cast<char*>(buffer.data()), length);

    return buffer;
}

}
