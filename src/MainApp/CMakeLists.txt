# 设置项目名称 
set(Lib_Name MainApp) 

# 设置搜索的根目录 
set(MAIN_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}) 

# 查找所有的.h和.cpp文件以及UI文件 
file(GLOB_RECURSE MAINAPP_HEADERS 
	"${MAIN_SRC_DIR}/CareRay/*.h" 
	"${MAIN_SRC_DIR}/Detector/*.h" 
	"${MAIN_SRC_DIR}/*.h"
) 

# 启用自动生成 MOC 文件
set(CMAKE_AUTOMOC ON)
# 启用自动生成 UI 头文件
set(CMAKE_AUTOUIC ON)  # 添加这一行

file(GLOB_RECURSE MAINAPP_SOURCES 
	${MAIN_SRC_DIR}/*.cpp
) 

file(GLOB_RECURSE MAINAPP_UI
    ${MAIN_SRC_DIR}/*.ui
) 
# 设置源文件变量 
set(MAINAPP_ALL_SRC
    ${MAINAPP_HEADERS}
    ${MAINAPP_SOURCES}
    ${MAINAPP_UI}
)
# 添加可执行文件编译目标 
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6) 
	qt_create_translation(QM_FILES ${PROJECT_SOURCES} ${CMAKE_SOURCE_DIR} ${TS_FILES}) 
	qt_add_executable(
		${ProjectName} 
		MANUAL_FINALIZATION 
        ${MAINAPP_ALL_SRC}
	) 
else() 
	add_executable(${ProjectName} 
      ${MAINAPP_ALL_SRC}
	) 
endif() 

target_include_directories(${ProjectName} PRIVATE
    ${MAIN_SRC_DIR}
    ${MAIN_SRC_DIR}/CareRay
    ${MAIN_SRC_DIR}/CareRay/include
	"D:/LibDir/zeromq/include"
)

# 链接所需的库 
target_link_libraries(
	${ProjectName} PRIVATE 
	Qt${QT_VERSION_MAJOR}::Widgets 
	Qt${QT_VERSION_MAJOR}::Gui 
	Qt${QT_VERSION_MAJOR}::PrintSupport 
	Qt${QT_VERSION_MAJOR}::Concurrent 
	Qt${QT_VERSION_MAJOR}::OpenGL 
	Qt${QT_VERSION_MAJOR}::OpenGLWidgets
	Qt${QT_VERSION_MAJOR}::Core5Compat 
	Qt${QT_VERSION_MAJOR}::WebSockets 
	Qt${QT_VERSION_MAJOR}::Sql 
	${OpenCV_LIBS} 
	# 添加 DCMTK 库 
	PRIVATE spdlog::spdlog 
	PRIVATE runtime::runtime 
	ws2_32 
	rpclib::rpc 
    $<$<CONFIG:Debug>:D:/LibDir/zeromq/lib/debug/libzmq-v143-mt-gd-4_3_6.lib>
    $<$<CONFIG:Release>:D:/LibDir/zeromq/lib/release/libzmq-v143-mt-4_3_6.lib>
	"${CMAKE_CURRENT_SOURCE_DIR}/CareRay/lib/x64/CareRayApi.lib"
	lz4::lz4
	
) 
# 确保将正确的runtime DLL复制到输出目录（区分Debug和Release版本）
add_custom_command(TARGET ${ProjectName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "复制 $<IF:$<CONFIG:Debug>,Debug,Release> 版本的runtime DLL"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<IF:$<CONFIG:Debug>,${RUNTIME_DIR}/x64-Debug/bin/runtimed.dll,${RUNTIME_DIR}/x64-Release/bin/runtime.dll>
        $<TARGET_FILE_DIR:${ProjectName}>
    COMMENT "Copying appropriate runtime DLL to output directory"
)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.XdScan)
endif()

# 设置输出的目标属性
set_target_properties(${ProjectName} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "ScanApp"
)

# Qt6 最终化
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${ProjectName})
endif()