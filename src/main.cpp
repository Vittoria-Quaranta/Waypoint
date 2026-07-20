#include "server.h"

#include <iostream>


int main()
{
    try
    {
        startServer();
    }

    catch(const std::exception& e)
    {
        std::cerr
            << "Waypoint failed to start: "
            << e.what()
            << std::endl;

        return 1;
    }


    return 0;
}