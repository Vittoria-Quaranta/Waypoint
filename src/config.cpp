#include "config.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Config loadConfig()
{
    const std::string path =
        "config/config.json";

    std::ifstream file(path);

    if (!file.is_open())
    {
        throw std::runtime_error(
            "Could not open config file: " + path);
    }

    json data;

    try
    {
        file >> data;
    }

    catch (const json::exception &e)
    {
        throw std::runtime_error(
            std::string("Invalid config JSON: ") + e.what());
    }

    Config config;

    try
    {
        config.databasePath =
            data.at("database_path")
                .get<std::string>();

        config.serverPort =
            data.at("server_port")
                .get<int>();
    }

    catch (const json::exception &e)
    {
        throw std::runtime_error(
            std::string("Missing or invalid config value: ") + e.what());
    }

    return config;
}