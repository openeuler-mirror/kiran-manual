cmake_minimum_required(VERSION 3.15)

project(kiran-manual VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set (kiran-manual test)
find_package(PkgConfig REQUIRED)
pkg_search_module(KLOG REQUIRED klog-qt5)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE FORMS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")

add_executable(kiran-manual
    ${HEADER}
    ${SOURCES}
    ${FORMS}
    ${PROJECT_SOURCE_DIR}/km-config.qrc)

target_link_libraries(kiran-manual PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets klog-qt5)


# install
install(TARGETS kiran-manual
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/manual-books
    DESTINATION share/kiran-manual/data
)
