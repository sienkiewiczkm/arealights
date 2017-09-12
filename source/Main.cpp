// arealights (2017)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include <iostream>
#include <cstdlib>
#include "Framework.hpp"
#include "Application.hpp"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, const char * argv[])
{
    START_EASYLOGGINGPP(argc, argv);
    LOG(INFO) << "Application arealights (2017) started.";
    fw::initialize(argc, argv);

    arealights::Application app;
    app.create();
    app.run();
    app.destroy();

    return EXIT_SUCCESS;
}
