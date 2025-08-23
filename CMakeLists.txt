cmake_minimum_required(VERSION 3.16)
project(MobileFarm VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick QuickControls2)

# ---- 可执行程序 ----
qt_add_executable(appMobileFarm
    main.cpp
)

# 纯 C++ 业务源码（不需要在 QML 里直接 new 的类）
target_sources(appMobileFarm PRIVATE
    # device
    device/device.h
    device/device.cpp
    device/devicemanager.h
    device/devicemanager.cpp
    device/devicescanner.h
    device/devicescanner.cpp

    # scrcpy
    scrcpy/scrcpysession.h
    scrcpy/scrcpysession.cpp
    scrcpy/scrcpyservice.h
    scrcpy/scrcpyservice.cpp

    # adb
    adb/adbrunner.h
    adb/adbrunner.cpp
)

# 让 #include "device/xxx.h" 这类路径可用
target_include_directories(appMobileFarm PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/device
)

# QML 模块（只放 QML 文件；若将来有 C++ 类型需要向 QML 注册，再放到 SOURCES）
qt_add_qml_module(appMobileFarm
    URI MobileFarm
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/DeviceScanDialog.qml
    SOURCES
        device/devicescanner.h
        device/devicescanner.cpp
)

# 依赖
target_link_libraries(appMobileFarm PRIVATE
    Qt6::Quick
    Qt6::QuickControls2
    # Qt6::Core  # 如果你想显式写一次也可以，但不要重复多次
)

# --- 复制 third_party 到 .app/Contents/MacOS ---
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
add_custom_command(TARGET appMobileFarm POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
            "${THIRD_PARTY_DIR}"
            "$<TARGET_FILE_DIR:appMobileFarm>/third_party"
    VERBATIM
)

# App bundle 属性（macOS/Windows）
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

# 让 IDE 按目录树显示
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "" FILES
    main.cpp
    qml/Main.qml
    qml/DeviceScanDialog.qml
    device/device.h device/device.cpp
    device/devicemanager.h device/devicemanager.cpp
    device/devicescanner.h device/devicescanner.cpp
    scrcpy/scrcpysession.h scrcpy/scrcpysession.cpp
    scrcpy/scrcpyservice.h scrcpy/scrcpyservice.cpp
    adb/adbrunner.h adb/adbrunner.cpp
)
