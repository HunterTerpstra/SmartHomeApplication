cmake_minimum_required(VERSION 3.23)
project(3307)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.80.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(3307
        harness.cpp Central.h Central.cpp Nodes/BaseNode.cpp Nodes/BaseNode.h Nodes/GraphNode.cpp Nodes/GraphNode.h
        Nodes/CentralNode.cpp Nodes/CentralNode.h Connections/Bluetooth.cpp Connections/Bluetooth.h Connections/Wifi.cpp
        Connections/Wifi.h Connections/NodePipe.cpp Connections/NodePipe.h Connections/Handlers/GetHandler.cpp
        Connections/Handlers/GetHandler.h Connections/Handlers/Handler.cpp Connections/Handlers/Handler.h
        Connections/Handlers/PostHandler.cpp Connections/Handlers/PostHandler.h Connections/Handlers/PutHandler.cpp
        Connections/Handlers/PutHandler.h Connections/Handlers/PatchHandler.cpp Connections/Handlers/PatchHandler.h
        Connections/Handlers/DeleteHandler.cpp Connections/Handlers/DeleteHandler.h Connections/Handlers/OptionsHandler.cpp
        Connections/Handlers/OptionsHandler.h Connections/Enumerators.h Parser/Parser.h Parser/Parser.cpp
        logger.h logger.cpp log_message.h log_message.cpp)



if(Boost_FOUND)
    target_link_libraries(3307 ${Boost_LIBRARIES} sqlite3)

else()
    target_link_libraries(3307 LINK_PUBLIC sqlite3)

endif()