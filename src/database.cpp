#include "database.h"
#include "config.h"

#include <sqlite3.h>
#include <iostream>
#include <filesystem>

sqlite3 *db = nullptr;

void initializeDatabase()
{
    Config config = loadConfig();

    // Make sure database folder exists
    std::filesystem::path dbPath(config.databasePath);

    if (dbPath.has_parent_path())
    {
        std::filesystem::create_directories(
            dbPath.parent_path());
    }

    int result = sqlite3_open(
        config.databasePath.c_str(),
        &db);

    if (result != SQLITE_OK)
    {
        throw std::runtime_error(
            "Could not open database");
    }

    const char *sql = R"(
        CREATE TABLE IF NOT EXISTS events
        (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            title TEXT NOT NULL
        );
    )";

    result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        nullptr);

    if (result != SQLITE_OK)
    {
        throw std::runtime_error(
            "Failed creating events table");
    }

    std::cout << "Database initialized\n";
}

void addEvent(Event event)
{
    const char *sql =
        "INSERT INTO events(date, title) VALUES(?, ?);";

    sqlite3_stmt *statement;

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr);

    sqlite3_bind_text(
        statement,
        1,
        event.date.c_str(),
        -1,
        SQLITE_TRANSIENT);

    sqlite3_bind_text(
        statement,
        2,
        event.title.c_str(),
        -1,
        SQLITE_TRANSIENT);

    sqlite3_step(statement);

    sqlite3_finalize(statement);
}

std::vector<Event> getEvents()
{
    std::vector<Event> events;

    const char *sql =
        "SELECT id, date, title FROM events;";

    sqlite3_stmt *statement;

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr);

    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        Event event;

        event.id =
            sqlite3_column_int(
                statement,
                0);

        event.date =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    statement,
                    1));

        event.title =
            reinterpret_cast<const char *>(
                sqlite3_column_text(
                    statement,
                    2));

        events.push_back(event);
    }

    sqlite3_finalize(statement);

    return events;
}

void deleteEvent(int id)
{

    const char *sql =
        "DELETE FROM events WHERE id = ?;";

    sqlite3_stmt *statement;

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr);

    sqlite3_bind_int(
        statement,
        1,
        id);

    sqlite3_step(statement);

    sqlite3_finalize(statement);
}
void updateEvent(int id, Event event)
{
    const char *sql =
        "UPDATE events SET date = ?, title = ? WHERE id = ?;";

    sqlite3_stmt *statement;

    sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr);

    sqlite3_bind_text(
        statement,
        1,
        event.date.c_str(),
        -1,
        SQLITE_TRANSIENT);

    sqlite3_bind_text(
        statement,
        2,
        event.title.c_str(),
        -1,
        SQLITE_TRANSIENT);

    sqlite3_bind_int(
        statement,
        3,
        id);

    sqlite3_step(statement);

    sqlite3_finalize(statement);
}