add_executable(simple_example simple_example.cpp)
target_link_libraries(simple_example PRIVATE qtl-logging)

add_executable(loggable_example loggable_example.cpp)
target_link_libraries(loggable_example PRIVATE qtl-logging)
