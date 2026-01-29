cmake_minimum_required(VERSION 3.16)

set(ProjectName ScanApp)
project(${ProjectName} VERSION 1.0.0 LANGUAGES C CXX)

# 设置调试信息生成选项
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
# 设置链接器选项，生成PDB文件
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#生成编译时间
string(TIMESTAMP COMPILE_TIME "%Y/%m/%d %H:%M")
string(TIMESTAMP TIME_YEAR %Y)
set(BUILD_TIME ${COMPILE_TIME})

#项目顶层目录
set(Project_Dir ${CMAKE_SOURCE_DIR})
message("-- Project_Dir: " ${Project_Dir})


# 多配置生成器的输出目录设置
if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "多配置生成器（Visual Studio）: ${CMAKE_CONFIGURATION_TYPES}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/LibOut)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>/LibOut)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/$<CONFIG>)
    # 判断 ${CMAKE_CONFIGURATION_TYPES} 是否包含 Debug 和 Release
    if("Debug" IN_LIST CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Debug 配置存在")
        SET(CMAKE_BUILD_TYPE Debug)
    endif()
    

    if("Release" IN_LIST CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Release 配置存在")
        SET(CMAKE_BUILD_TYPE Release)
    endif()
   
else()
    # 保留现有的单配置设置
    message(STATUS "单配置生成器，构建类型: ${CMAKE_BUILD_TYPE}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/LibOut)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

set(PROJECT_LIBRARY_OUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(PROJECT_RUNTIME_OUT_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


set(CMAKE_PREFIX_PATH D:/Qt/6.4.3/msvc2019_64)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Gui Concurrent Sql  Xml Network WebSockets OpenGL OpenGLWidgets Core5Compat PrintSupport LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Gui Concurrent  Sql Xml Network WebSockets OpenGL OpenGLWidgets Core5Compat PrintSupport LinguistTools)

# 在find_package(Qt...)之后添加
get_target_property(QtCore_location Qt${QT_VERSION_MAJOR}::Core LOCATION)
message(STATUS "Qt Core 库位置: ${QtCore_location}")

# 允许用户指定runtime路径或使用默认路径
set(RUNTIME_DIR "D:/LibDir/runtime" CACHE PATH "Path to runtime installation directory")

 # 单配置生成器使用原有方式
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        find_package(runtime REQUIRED PATHS "${RUNTIME_DIR}/x64-Debug/lib/cmake/runtime")
    else()
        find_package(runtime REQUIRED PATHS "${RUNTIME_DIR}/x64-Release/lib/cmake/runtime")
    endif()
    
    set(RUNTIME_LIB runtime::runtime)

# 如果需要显式包含runtime头文件
include_directories(${runtime_INCLUDE_DIRS})
message(STATUS "runtime_INCLUDE_DIRS: ${runtime_INCLUDE_DIRS}")

# rpclib包
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(RPCLIB REQUIRED PATHS "D:/LibDir/rpclib/debug/lib/cmake/rpclib")
else()
   find_package(RPCLIB REQUIRED PATHS "D:/LibDir/rpclib/release/lib/cmake/rpclib")
endif()

# lz4包
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(LZ4 REQUIRED PATHS "D:/LibDir/lz4/debug/lib/cmake/lz4")
else()
   find_package(LZ4 REQUIRED PATHS "D:/LibDir/lz4/release/lib/cmake/lz4")
endif()

#find dcmtk
find_package(DCMTK REQUIRED PATHS "D:/LibDir/dcmtk3.6/cmake")

# 添加 DCMTK 的头文件路径
include_directories(${DCMTK_INCLUDE_DIRS})


set(DCMTK_LIBRARY_DIR "D:/LibDir/dcmtk3.6/lib")

# 添加 DCMTK 库路径
link_directories(${DCMTK_LIBRARY_DIR})

# 打印 DCMTK 信息以便调试
message(STATUS "DCMTK_INCLUDE_DIRS: ${DCMTK_INCLUDE_DIRS}")
message(STATUS "DCMTK_LIBRARIES: ${DCMTK_LIBRARIES}")
message(STATUS "DCMTK_LIBRARY_DIR: ${DCMTK_LIBRARY_DIR}")

# ================= DCMWRITER =================

# 添加 DCMWRITER 的头文件路径
include_directories("D:/LibDir/CareRayPlugin/include")

# 设置 DCMWRITER 库路径
set(CareRayPlugin_LIBRARY_DIR "D:/LibDir/CareRayPlugin/debug/lib")

# 添加 DCMWRITER 库路径
link_directories(${CareRayPlugin_LIBRARY_DIR})

# spdlog2库
# 设置spdlog2库的路径
#find_package(fmt REQUIRED PATHS  ${CMAKE_SOURCE_DIR}/3rd/bin/spdlog2/lib/cmake/fmt)
#find_package(spdlog REQUIRED PATHS  ${CMAKE_SOURCE_DIR}/3rd/bin/spdlog2/lib/cmake/spdlog-2)
#message ("spdloglib:" spdlog::spdlog)

# spdlog1.x库 2没有刷新接口 不会用
find_package(spdlog REQUIRED PATHS  "D:/LibDir/spdlog1/lib/cmake/spdlog")

#OpenCV
#find_package (OpenCV REQUIRED PATHS "D:/LibDir/opencv/opencv4.90/build/x64/vc16/lib")
find_package (OpenCV REQUIRED PATHS "D:/LibDir/opencv/opencv4.90/build/x64/vc16/lib")

include_directories( ${OpenCV_INCLUDE_DIR})


#添加头文件
include_directories(
    ${Project_Dir}/Detector
    D:/LibDir/qt-solutions/qtpropertybrowser/src
    D:/LibDir/qt-solutions/qtsingleapplication/src
    )

#添加lib
add_definitions(-DQT_QTSINGLEAPPLICATION_IMPORT -DQT_QTPROPERTYBROWSER_IMPORT -DTILED_WINDOWS_LAYOUT)

# 为多配置生成器（如Visual Studio）设置库
set(DEBUG_LIBS 
    "D:/LibDir/qt-solutions/qtpropertybrowser/lib/qtpropertybrowserd.lib"
    "D:/LibDir/qt-solutions/qtsingleapplication/lib/Qt6SingleApplicationd.lib"
    "D:/LibDir/spdlog1/lib/spdlogd.lib"
    # 其他debug库...
)

set(RELEASE_LIBS
    "D:/LibDir/qt-solutions/qtpropertybrowser/lib/qtpropertybrowser.lib"
    "D:/LibDir/qt-solutions/qtsingleapplication/lib/Qt6SingleApplication.lib"
    "D:/LibDir/spdlog1/lib/spdlog.lib"
    # 其他release库...
)

add_subdirectory(${Project_Dir}/src)

set(PROJECT_SOURCES
        ${All_Src}
)

include(GNUInstallDirs)
install(TARGETS ScanApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
