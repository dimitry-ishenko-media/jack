# Library

find_package(PkgConfig REQUIRED)
pkg_search_module(JACK REQUIRED IMPORTED_TARGET jack)

# add server libs to PkgConfig::JACK
pkg_get_variable(JACK_SERVER_LINK_LIBRARIES jack server_libs)
target_link_libraries(PkgConfig::JACK INTERFACE ${JACK_SERVER_LINK_LIBRARIES})

set(HEADERS
    jack++/client.hpp
    jack++/driver.hpp
    jack++/error.hpp
    jack++/param.hpp
    jack++/port.hpp
    jack++/server.hpp
    jack++/types.hpp
)
set(OVERALL_HEADER jack++.hpp)

set(SOURCES
    client.cpp
    driver.cpp
    error.cpp
    param.cpp
    port.cpp
    server.cpp
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
