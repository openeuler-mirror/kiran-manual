cmake_minimum_required(VERSION 3.5)

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


set(PROJECT_SOURCE_DIR src)

set(HEADER
	${PROJECT_SOURCE_DIR}/
        window.h
	navigation.h
	article.h
        markdown-parser.h
        searcher.h	
)
set(SOURCES
	${PROJECT_SOURCE_DIR}/
    	main.cpp
    	window.cpp
    	navigation.cpp
    	article.cpp
    	markdown-parser.cpp
    	searcher.cpp
)

add_executable(kiran-manual
        src/window.h
	src/navigation.h
	src/article.h
        src/markdown-parser.h
        src/searcher.h	
    	src/main.cpp
    	src/window.cpp
    	src/navigation.cpp
    	src/article.cpp
    	src/markdown-parser.cpp
    	src/searcher.cpp
	src/window.ui
	src/navigation.ui
	src/article.ui
	km-config.qrc
)
target_link_libraries(kiran-manual PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
