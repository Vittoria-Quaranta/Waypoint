#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([](){
        return "Hello from C++ on Mac!";
    });

    app.port(8080)
       .multithreaded()
       .run();
}