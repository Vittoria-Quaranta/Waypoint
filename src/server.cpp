#include "server.h"
#include "crow.h"
#include "event.h"
#include "database.h"
#include "config.h"

#include <fstream>
#include <sstream>

std::string readFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void startServer()
{
    Config config = loadConfig();

    initializeDatabase();

    crow::SimpleApp app;

    crow::mustache::set_base("templates");

    // Homepage
    CROW_ROUTE(app, "/")
    ([]()
     {
        auto page =
            crow::mustache::load("index.html");

        return page.render(); });

    // Status test
    CROW_ROUTE(app, "/api/status")
    ([]()
     { return crow::json::wvalue{
           {"status", "running"},
           {"app", "Waypoint"}}; });

    // GET ALL EVENTS
    CROW_ROUTE(app, "/api/events")
    ([]()
     {
        auto events = getEvents();


        crow::json::wvalue result(
            crow::json::type::List
        );


        int index = 0;


        for(auto& event : events)
        {
            result[index]["id"] =
                event.id;


            result[index]["date"] =
                event.date;


            result[index]["title"] =
                event.title;


            index++;
        }


        return result; });

    // CREATE EVENT
    CROW_ROUTE(app, "/api/events")
        .methods(crow::HTTPMethod::Post)([](const crow::request &request)
                                         {

        auto body =
            crow::json::load(request.body);


        if(!body)
        {
            return crow::response(400);
        }


        Event event;


        event.date =
            body["date"].s();


        event.title =
            body["title"].s();


        addEvent(event);


        return crow::response(200); });

    // DELETE EVENT
    CROW_ROUTE(app, "/api/events/<int>")
        .methods(crow::HTTPMethod::Delete)([](int id)
                                           {
                                               deleteEvent(id);

                                               return crow::response(200);
                                           });

    // STATIC FILES
    CROW_ROUTE(app, "/static/<string>")
    ([](std::string filename)
     {
         crow::response response;

         std::string path =
             "static/" + filename;

         std::ifstream file(
             path,
             std::ios::binary);

         if (!file.is_open())
         {
             response.code = 404;
             return response;
         }

         std::stringstream buffer;

         buffer << file.rdbuf();

         response.body =
             buffer.str();

         if (filename.find(".css") != std::string::npos)
         {
             response.set_header(
                 "Content-Type",
                 "text/css");
         }

         else if (filename.find(".js") != std::string::npos)
         {
             response.set_header(
                 "Content-Type",
                 "application/javascript");
         }

         return response;
     });

    CROW_ROUTE(app, "/api/events/<int>")
        .methods(crow::HTTPMethod::Put)([](const crow::request &request, int id)
                                        {
                                            auto body =
                                                crow::json::load(request.body);

                                            if (!body)
                                            {
                                                return crow::response(400);
                                            }

                                            Event event;

                                            event.date =
                                                body["date"].s();

                                            event.title =
                                                body["title"].s();

                                            updateEvent(id, event);

                                            return crow::response(200);
                                        });

    app.port(config.serverPort)
        .multithreaded()
        .run();
}