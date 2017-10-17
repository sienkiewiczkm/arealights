// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Common.hpp"

#include <cmath>
#include <fstream>
#include <streambuf>
#include "Logging.hpp"

using namespace std;

double fw::pi()
{
    return std::acos(-1.0);
}

float fw::pif()
{
    return std::acos(-1.0f);
}

std::string fw::loadASCIITextFile(const std::string &filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        LOG(ERROR) << "Requested file \"" << filepath << "\" cannot be opened.";
        // todo: don't return empty string, it's not nice
        return "";
    }

    string output;

    file.seekg(0, ios::end);
    output.reserve(file.tellg());
    file.seekg(0, ios::beg);

    output.assign(
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>()
    );

    return output;
}

const char *fw::transformStringToCStr(const std::string &str)
{
    return str.c_str();
}
