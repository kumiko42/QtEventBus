# TEMPLATE = subdirs
# SUBDIRS = \
#    Tests \
#    QtEventBus

cmake_minimum_required(VERSION 3.8)

include(FetchContent)

project(qt-event-bus)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.15/msvc2019_64/lib/cmake")

set(QT_LIBS Core Quick)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS ${QT_LIBS} REQUIRED)
set(QT_LIBS_WITH_PREFIX)
foreach(lib ${QT_LIBS})
    list(APPEND QT_LIBS_WITH_PREFIX "Qt::${lib}")
endforeach()


FetchContent_Declare(qtpromise
        GIT_REPOSITORY https://github.com/simonbrunel/qtpromise.git
        GIT_TAG v0.7.0
        GIT_SHALLOW true
)
FetchContent_MakeAvailable(qtpromise)

FetchContent_Declare(qt-composition
        SOURCE_DIR C:/Repos/QtComposition
)
FetchContent_MakeAvailable(qt-composition)
include_directories(${qt-composition_SOURCE_DIR}/QtComposition)

if (WIN32)
    set(QT_EVENT_BUS_BIN_OUTPUT ${PROJECT_BINARY_DIR}/bin)
    message("QT_EVENT_BUS_BIN_OUTPUT: ${QT_EVENT_BUS_BIN_OUTPUT}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${QT_EVENT_BUS_BIN_OUTPUT})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${QT_EVENT_BUS_BIN_OUTPUT})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${QT_EVENT_BUS_BIN_OUTPUT})
    set(CMAKE_DEBUG_POSTFIX "")
endif()

add_subdirectory(Tests)
add_subdirectory(QtEventBus)
