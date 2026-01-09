cmake_minimum_required(VERSION 4.0)

add_executable(assignment2)

target_sources(assignment2 PRIVATE
    main.cpp
    memoryAllocator.cpp
    memoryDemo.cpp
    memoryManager.cpp
)

target_compile_options(assignment2 PRIVATE
    -O0
    -g3
    -fsanitize=address
    -fsanitize=undefined
)

target_link_options(assignment2 PRIVATE
    -fsanitize=address
    -fsanitize=undefined
)

