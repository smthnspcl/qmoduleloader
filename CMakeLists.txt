cmake_minimum_required(VERSION 3.2)
project(qmoduleloader)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Qml REQUIRED)

add_library(qmoduleloader SHARED qmoduleloader.h qmoduleloader.h)

set_target_properties(qmoduleloader PROPERTIES PUBLIC_HEADER "qmoduleloader.h")

target_link_libraries(qmoduleloader
        Qt5::Core Qt5::Qml)

install(TARGETS qmoduleloader
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/qmoduleloader)