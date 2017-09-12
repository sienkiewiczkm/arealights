// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#pragma once

#include <cmath>
#include <string>

namespace fw
{

double pi();
float pif();

std::string loadASCIITextFile(const std::string &filepath);

const char *transformStringToCStr(const std::string &str);

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

}
