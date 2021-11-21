target_sources(WooD PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
                    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/e_exceptions.cpp")

add_subdirectory(wad)
add_subdirectory(mem)
add_subdirectory(map)