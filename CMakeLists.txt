cmake_minimum_required(VERSION 3.5)

project(CellAutomatV2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_MIN_VERSION 6.0)
find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

qt_add_executable(CellAutomatV2
    ${PROJECT_SOURCES}
    cells.h cells.cpp
    celltype.h celltype.cpp

    globalvars.cpp
    globalvars.h
    myqgraphicsview.h myqgraphicsview.cpp
    settingswidget.h settingswidget.cpp
    addcellwidget.h addcellwidget.cpp
    typecreationwidget.h typecreationwidget.cpp
    celltickprocesser.h celltickprocesser.cpp
)

target_link_libraries(CellAutomatV2 PRIVATE Qt6::Widgets)
target_include_directories(CellAutomatV2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(CellAutomatV2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CellAutomatV2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

option(CELLAUTOMAT_BUILD_TESTS "Сборка тестов и бенчмарка из каталога tests" ON)
if(CELLAUTOMAT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../tests ${CMAKE_BINARY_DIR}/tests)
endif()
