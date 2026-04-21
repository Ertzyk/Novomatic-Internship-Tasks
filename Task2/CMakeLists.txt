cmake_minimum_required(VERSION 3.15)
project(NovomaticTask2)

find_package(nlohmann_json REQUIRED)
find_package(SFML REQUIRED COMPONENTS graphics window system)

add_executable(blueprint_layout 
    src/main.cpp 
    src/Graph.cpp
)

target_link_libraries(blueprint_layout PRIVATE 
    nlohmann_json::nlohmann_json
    sfml-graphics
    sfml-window
    sfml-system
)