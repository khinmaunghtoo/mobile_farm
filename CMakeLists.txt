cmake_minimum_required(VERSION 3.16)
project(MobileFarm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 必须包含 Qml
find_package(Qt6 6.8 REQUIRED COMPONENTS Qml Quick QuickControls2)

# ---- 可执行程序 ----
qt_add_executable(appMobileFarm
    main.cpp
)

# ---- QML 模块 ----
qt_add_qml_module(appMobileFarm
    URI MobileFarm
    VERSION 1.0
    QML_FILES
        Main.qml
        DeviceScanDialog.qml
    SOURCES
        devicescanner.h
        devicescanner.cpp
)

# 纯 C++ 业务源码
target_sources(appMobileFarm PRIVATE
    device.h device.cpp
    devicemanager.h devicemanager.cpp
    scrcpysession.h scrcpysession.cpp
    scrcpyservice.h scrcpyservice.cpp
    adbrunner.h adbrunner.cpp
)

# ---- 链接依赖 ----
target_link_libraries(appMobileFarm PRIVATE
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
)

# --- 复制 third_party ---
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
add_custom_command(TARGET appMobileFarm POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${THIRD_PARTY_DIR}"
            "$<TARGET_FILE_DIR:appMobileFarm>/third_party"
    VERBATIM
)

# App bundle 属性
set_target_properties(appMobileFarm PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# 安装规则
include(GNUInstallDirs)
install(TARGETS appMobileFarm
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
