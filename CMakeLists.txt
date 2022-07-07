cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

project(OTAUpdate
        VERSION 2019.1.0
        DESCRIPTION "OTA UPDATE"
        LANGUAGES CXX)

string(APPEND CMAKE_CXX_FLAGS " -fno-strict-aliasing -pthread ")

find_package(Dependencies)
install_dependency(https://github.com/germanespinosa/easy-tcp)
include_directories(dependencies/easy-tcp/include)


install_dependency(https://github.com/germanespinosa/params-cpp Params-cpp)

add_executable(OTAUpdate src/OTAUpdate.cpp)

target_link_libraries( OTAUpdate
        LINK_PUBLIC
        easy-tcp
        params-cpp)