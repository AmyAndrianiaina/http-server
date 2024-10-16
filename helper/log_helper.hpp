#ifndef LOG_HELPER
#define LOG_HELPER

#include <iostream>

namespace http
{
    void log(const std::string& message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string& errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

#endif