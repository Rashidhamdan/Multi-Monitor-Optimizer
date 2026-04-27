cmake_minimum_required(VERSION 3.16)

set(TEST_TARGET MultiMonitorOptimizerTests)

add_executable(${TEST_TARGET}
    test_calculator.cpp
    ../core/monitor.cpp
    ../core/calculator.cpp
    ../core/optimizer.cpp
)

target_include_directories(${TEST_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

target_link_libraries(${TEST_TARGET} PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Core
)

add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
