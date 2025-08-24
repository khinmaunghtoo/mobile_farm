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

# ---- 包含目录 ----
target_include_directories(appMobileFarm PRIVATE 
    src/device
    src/scrcpy
    src/adb
)

# ---- QML 模块 ----
qt_add_qml_module(appMobileFarm
    URI MobileFarm
    VERSION 1.0
    QML_FILES
        src/qml/Main.qml
        src/qml/DeviceScanDialog.qml
    SOURCES
        src/device/devicescanner.h
        src/device/devicescanner.cpp
        src/device/devicemanager.h
        src/device/devicemanager.cpp
        src/device/device.h
        src/device/device.cpp
)

# 纯 C++ 业务源码
target_sources(appMobileFarm PRIVATE
# device
    src/device/device.h src/device/device.cpp
    src/device/devicemanager.h src/device/devicemanager.cpp
    src/device/devicescanner.h src/device/devicescanner.cpp
# scrcpy
    src/scrcpy/scrcpysession.h src/scrcpy/scrcpysession.cpp
    src/scrcpy/scrcpyservice.h src/scrcpy/scrcpyservice.cpp
# adb
    src/adb/adbrunner.h src/adb/adbrunner.cpp
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


