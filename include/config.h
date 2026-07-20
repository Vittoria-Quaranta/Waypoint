#pragma once

#include <string>


struct Config
{
    std::string databasePath;
    int serverPort;
};


Config loadConfig();