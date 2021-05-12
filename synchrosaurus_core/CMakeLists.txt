cmake_minimum_required(VERSION 3.5)

project(synchrosaurus_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

FILE(GLOB_RECURSE LibFiles "include/*.hpp")
add_custom_target(headers SOURCES ${LibFiles})

add_executable(synchrosaurus_core
  src/main.cpp
  src/directory_scanner.cpp
  src/file_mode_printer.cpp
  src/sqlite.cpp
)

target_include_directories(synchrosaurus_core PUBLIC include)
target_link_libraries(synchrosaurus_core SQLite::SQLite3)
