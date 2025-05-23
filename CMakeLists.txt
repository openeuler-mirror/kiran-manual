cmake_minimum_required(VERSION 3.2)

project(
  kiran-manual
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 QT 库
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets LinguistTools)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
find_package(
  Qt${QT_VERSION}
  COMPONENTS ${REQUIRED_LIBS}
  REQUIRED)
# 查找 Kiran 相关组件
find_package(PkgConfig REQUIRED)
pkg_search_module(KLOG REQUIRED klog-qt5)
pkg_search_module(KIRAN_INTEGRATION_THEME REQUIRED kiran-integration-theme)
pkg_search_module(KIRAN_WIDGETS REQUIRED kiranwidgets-qt5)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(RESOURCE ./resources/kiran-manual-resources.qrc)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE FORMS "${PROJECT_SOURCE_DIR}/*.ui")

set(TS_FILES ./translations/kiran-manual.zh_CN.ts)

set(KIRAN_MANUAL_ICON_INSTALL_PATH ${INSTALL_DATADIR}/icons)

include(install-location-define.cmake)
configure_file(${CMAKE_SOURCE_DIR}/data/config.h.in
               ${CMAKE_BINARY_DIR}/config.h)
configure_file(${CMAKE_SOURCE_DIR}/data/${PROJECT_NAME}.desktop.in
               ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop)
# 生成 .qm 翻译文件
qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})

add_executable(${PROJECT_NAME} ${HEADER} ${SOURCES} ${FORMS} ${RESOURCE}
                               ${TS_FILES} ${QM_FILES})

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}
          ${KLOG_INCLUDE_DIRS} ${KIRAN_INTEGRATION_THEME_INCLUDE_DIRS}
          ${KIRAN_WIDGETS_INCLUDE_DIRS})

target_link_libraries(
  ${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED} ${KLOG_LIBRARIES}
  ${KIRAN_INTEGRATION_THEME_LIBRARIES} ${KIRAN_WIDGETS_LIBRARIES})

# 安装二进制文件
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
# 安装翻译
install(FILES ${QM_FILES}
        DESTINATION ${INSTALL_DATADIR}/${PROJECT_NAME}/translations)
# 安装 文档库配置文件
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/km.ini
        DESTINATION ${INSTALL_DATADIR}/${PROJECT_NAME}/data)
# 安装 文档库
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/documents
        DESTINATION ${INSTALL_DATADIR}/${PROJECT_NAME}/data)
# 安装 kiran-manual.desktop
install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.desktop"
        DESTINATION "${INSTALL_DATADIR}/applications/")
# 安装 kiran-icon icon文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/hicolor
        DESTINATION "${INSTALL_DATADIR}/icons")
