cmake_minimum_required(VERSION 3.16) # focal
project(jack++ VERSION 0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_search_module(JACK REQUIRED IMPORTED_TARGET jack)

# add server libs to PkgConfig::JACK
pkg_get_variable(JACK_SERVER_LINK_LIBRARIES jack server_libs)
target_link_libraries(PkgConfig::JACK INTERFACE ${JACK_SERVER_LINK_LIBRARIES})

set(HEADERS
    src/client.hpp
    src/driver.hpp
    src/error.hpp
    src/param.hpp
    src/server.hpp
)
set(OVERALL_HEADER src/jack++.hpp)

set(SOURCES
    src/client.cpp
    src/driver.cpp
    src/error.cpp
    src/param.cpp
    src/server.cpp
)

include(GNUInstallDirs)

add_library(${PROJECT_NAME} OBJECT ${HEADERS} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_link_libraries(${PROJECT_NAME} PUBLIC PkgConfig::JACK)

add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}>)
set_target_properties(${PROJECT_NAME}_shared PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_VERSION_MAJOR}
)

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}>)
set_target_properties(${PROJECT_NAME}_static PROPERTIES
    OUTPUT_NAME ${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}_shared ${PROJECT_NAME}_static DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${OVERALL_HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES LICENSE.md README.md DESTINATION ${CMAKE_INSTALL_DOCDIR})
