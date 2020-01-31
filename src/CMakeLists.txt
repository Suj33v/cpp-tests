include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(
    main
    algorithms.cpp
    boost.cpp
    main.cpp
    pointers.cpp
    strings.cpp
)

target_link_libraries(main ${LIBS})
