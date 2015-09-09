cmake_minimum_required(VERSION 3.2)

set(SOURCE_FILES uno.c)

add_executable(uno uno.c window.c function.c)
add_executable(assignment2 assignment2.c)
add_executable(demo demo.c function.c)
target_link_libraries(uno ${CURSES_LIBRARIES})
target_link_libraries(assignment2 ${CURSES_LIBRARIES})
