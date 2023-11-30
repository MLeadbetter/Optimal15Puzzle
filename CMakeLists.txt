project(Optimal15Puzzle)
cmake_minimum_required(VERSION 3.14)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# Configure project
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -O3 -march=native -flto")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -g")

set(SHARED_SOURCES
    ${CMAKE_SOURCE_DIR}/grid.h
    ${CMAKE_SOURCE_DIR}/simplified_grid.h
    ${CMAKE_SOURCE_DIR}/solver.h
    ${CMAKE_SOURCE_DIR}/simplified_solver.h
    ${CMAKE_SOURCE_DIR}/bucket_queue.h
    ${CMAKE_SOURCE_DIR}/uint64_hash_set.h
    ${CMAKE_SOURCE_DIR}/solution.h
    ${CMAKE_SOURCE_DIR}/lower_bound_tables.h
    ${CMAKE_SOURCE_DIR}/grid_converter.h

    ${CMAKE_SOURCE_DIR}/functions.cpp
)

set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${SHARED_SOURCES}
)

set(TEST_SOURCES
    ${CMAKE_SOURCE_DIR}/test_grid.cpp
    ${CMAKE_SOURCE_DIR}/test_bucket_queue.cpp
    ${CMAKE_SOURCE_DIR}/test_solver.cpp
    ${CMAKE_SOURCE_DIR}/test_hash_set.cpp
    ${CMAKE_SOURCE_DIR}/test_solution.cpp
    ${CMAKE_SOURCE_DIR}/test_grid_converter.cpp
    ${SHARED_SOURCES}
)

include_directories(
    ${CMAKE_SOURCE_DIR} include
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

add_executable(test_${PROJECT_NAME} ${TEST_SOURCES})
target_link_libraries(test_${PROJECT_NAME} gtest_main)
