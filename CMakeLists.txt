cmake_minimum_required(VERSION 3.16)

project(Multi-Monitor-Optimizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick Qml)

# =========================================================
# Additional global settings
# =========================================================

set(PROJECT_TARGET Multi-Monitor-Optimizer)

message(STATUS "Configuring project: ${PROJECT_NAME}")
message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "Using Qt major version: ${QT_VERSION_MAJOR}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# output directories for cleaner build layout
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# =========================================================
# Project source structure
# =========================================================

set(APP_SOURCES
    app/main.cpp
)

set(CORE_SOURCES
    core/monitor.cpp
    core/optimizer.cpp
    core/calculator.cpp
)

set(CORE_HEADERS
    core/monitor.h
    core/calculationresult.h
    core/optimizer.h
    core/calculator.h
)

set(GUI_SOURCES
    gui/mainwindow.cpp
    gui/monitorwidget.cpp
    gui/resultswidget.cpp
    gui/resultitemwidget.cpp
)

set(GUI_HEADERS
    gui/mainwindow.h
    gui/monitorwidget.h
    gui/resultswidget.h
    gui/resultitemwidget.h 
)

set(UI_FILES
    gui/mainwindow.ui
)

set(RESOURCE_FILES
    resources/resources.qrc
)

set(PROJECT_SOURCES
    ${APP_SOURCES}
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${GUI_SOURCES}
    ${GUI_HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

# =========================================================
# Main executable
# =========================================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_TARGET}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()

target_link_libraries(Multi-Monitor-Optimizer PRIVATE Qt${QT_VERSION_MAJOR}::Core)
if(ANDROID)
        add_library(${PROJECT_TARGET} SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(${PROJECT_TARGET}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# =========================================================
# Include directories
# =========================================================

target_include_directories(${PROJECT_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

# =========================================================
# Link Qt libraries
# =========================================================

target_link_libraries(${PROJECT_TARGET} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
target_link_libraries(${PROJECT_TARGET} PRIVATE
    Qt${QT_VERSION_MAJOR}::Charts
)
target_link_libraries(Multi-Monitor-Optimizer PRIVATE
    Qt6::Quick
    Qt6::Qml
)

# =========================================================
# Compiler warnings
# =========================================================

if(MSVC)
    target_compile_options(${PROJECT_TARGET} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_TARGET} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# =========================================================
# Optional compile definitions
# =========================================================

target_compile_definitions(${PROJECT_TARGET} PRIVATE
    $<$<CONFIG:Debug>:MMO_DEBUG_BUILD>
    $<$<CONFIG:Release>:MMO_RELEASE_BUILD>
)

# =========================================================
# Source groups for IDEs (Visual Studio / Qt Creator friendly)
# =========================================================

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/app PREFIX "app" FILES ${APP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/core PREFIX "core" FILES ${CORE_SOURCES} ${CORE_HEADERS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/gui PREFIX "gui" FILES ${GUI_SOURCES} ${GUI_HEADERS} ${UI_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/resources PREFIX "resources" FILES ${RESOURCE_FILES})

# =========================================================
# Target properties
# =========================================================

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Multi-Monitor-Optimizer)
endif()

set_target_properties(${PROJECT_TARGET} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# =========================================================
# Install rules
# =========================================================

include(GNUInstallDirs)

install(TARGETS ${PROJECT_TARGET}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Optional: install docs
install(FILES
    README.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs/architecture.md")
    install(FILES
        docs/architecture.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs/formulas.md")
    install(FILES
        docs/formulas.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )
endif()

# =========================================================
# Checks for expected project folders
# =========================================================

foreach(required_dir app core gui resources)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${required_dir}")
        message(WARNING "Expected directory '${required_dir}' does not exist yet.")
    endif()
endforeach()

# =========================================================
# Tests
# =========================================================

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
        message(STATUS "Tests enabled")
    else()
        message(STATUS "tests/CMakeLists.txt not found - skipping tests")
    endif()
endif()

# =========================================================
# Finalize executable for Qt6
# =========================================================

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_TARGET})
endif()
