add_subdirectory(MainApp)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
)

set(All_Src
    ${MainApp_Src}
    PARENT_SCOPE
)
