cmake_minimum_required(VERSION 3.16)

project(SecureFileEncryptor VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(SecureFileEncryptor WIN32
    main.cpp
    MainWindow.h
    MainWindow.cpp
    crypto.h
    crypto.cpp
)

target_link_libraries(SecureFileEncryptor PRIVATE Qt6::Widgets)

set_target_properties(SecureFileEncryptor PROPERTIES
    WIN32_EXECUTABLE TRUE
)
