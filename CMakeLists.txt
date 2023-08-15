cmake_minimum_required(VERSION 3.15)

project(kiran-manual VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 定义查找 QT 库
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(Qt${QT_VERSION} COMPONENTS
        ${REQUIRED_LIBS} REQUIRED)

find_package(PkgConfig REQUIRED)
# 查找 Kiran 相关库
pkg_search_module(KLOG REQUIRED klog-qt5)
pkg_search_module(KIRAN_STYLE REQUIRED kiran-style-helper)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE FORMS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui")

add_executable(${PROJECT_NAME}
    ${HEADER}
    ${SOURCES}
    ${FORMS}
    ${PROJECT_SOURCE_DIR}/km-config.qrc)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}
        ${KLOG_INCLUDE_DIRS}
        ${KIRAN_STYLE_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME}
        ${REQUIRED_LIBS_QUALIFIED}
        ${KLOG_LIBRARIES}
        ${KIRAN_STYLE_LIBRARIES}
        )

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/manual-books
    DESTINATION share/${PROJECT_NAME}/data
)
