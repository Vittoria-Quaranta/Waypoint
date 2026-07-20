cmake_minimum_required(VERSION 3.15)

project(Waypoint)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(Crow)

find_package(nlohmann_json REQUIRED)

add_executable(
    Waypoint
    src/main.cpp
    src/server.cpp
    src/database.cpp
    src/config.cpp
)
find_package(SQLite3 REQUIRED)


target_link_libraries(
    Waypoint
    PRIVATE
    Crow::Crow
    SQLite::SQLite3
    nlohmann_json::nlohmann_json
)

target_include_directories(
    Waypoint PRIVATE
    Crow/include
    include
)


add_custom_command(
    TARGET Waypoint POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/templates
    ${CMAKE_BINARY_DIR}/templates
)

add_custom_command(
    TARGET Waypoint POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/templates
    ${CMAKE_BINARY_DIR}/templates
)

add_custom_command(
    TARGET Waypoint POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/static
    ${CMAKE_BINARY_DIR}/static
)
add_custom_command(
    TARGET Waypoint POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config
    ${CMAKE_BINARY_DIR}/config
)