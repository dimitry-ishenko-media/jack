# example

add_executable(server server.cpp)
target_link_libraries(server jack++::jack++)

add_executable(sidetone sidetone.cpp)
target_link_libraries(sidetone jack++::jack++)
