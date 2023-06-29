cmake_minimum_required(VERSION 3.5)

project(kiran-manual LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/article-page.cpp
    src/home-page.cpp
    src/main-window.cpp
    src/main.cpp
    src/markdown-parser.cpp
    src/searcher.cpp
)
set(HEADER_DIR src)
set(HEADER_FILES
	${HEADER_DIR}/
	article-page.h
	home-page.h
        main-window.h
        markdown-parser.h
        searcher.h	
)

add_executable(kiran-manual
	${SOURCES}
	forms/article-page.ui
	forms/home-page.ui
	forms/main-window.ui
	km-config.qrc
)
foreach(HEADER_FILE ${HEADER_FILES})
    message("Adding header file: ${HEADER_FILE}")
    target_include_directories(kiran-manual PRIVATE ${HEADER_FILE})
endforeach()
target_link_libraries(kiran-manual PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
