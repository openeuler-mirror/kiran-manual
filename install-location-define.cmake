include(GNUInstallDirs)

set(INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
set(INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR})

# desktop安装路径
set(DESKTOP_INSTALL_DIR ${INSTALL_DATADIR}/${PROJECT_NAME}/desktop)
# 译文件安装路径
set(TRANSLATION_INSTALL_DIR ${INSTALL_DATADIR}/${PROJECT_NAME}/translations/)
