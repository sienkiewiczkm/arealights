// framework (2016)
// Kamil Sienkiewicz <sienkiewiczkm@gmail.com>

#include "Framework.hpp"
#include "Logging.hpp"

namespace fw
{

void initialize(int argc, const char *argv[])
{
    const char *frameworkLoggerName = "framework";
    el::Loggers::getLogger(frameworkLoggerName);
    CLOG(INFO, frameworkLoggerName) << "Framework initialized.";
}

}
