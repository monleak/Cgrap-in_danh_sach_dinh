set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
add_subdirectory(cgraph)

add_executable(20194562 20194562.c)
target_link_libraries(20194562 cgraph)

add_subdirectory(libfdr)
target_link_libraries(20194562 fdr)