project(imgui-docking)

file(GLOB IMGUI_DOCKING_PATHS ${CMAKE_CURRENT_LIST_DIR}/imgui-*-docking)
list(FILTER IMGUI_DOCKING_PATHS EXCLUDE REGEX "\\.cmake$")
list(GET IMGUI_DOCKING_PATHS 0 IMGUI_DOCKING_PATH)

if(IMGUI_PATHS)
    list(GET IMGUI_PATHS 0 IMGUI_PATH)
endif()

if(NOT IMGUI_DOCKING_PATH)
    file(DOWNLOAD 
        https://github.com/ocornut/imgui/archive/refs/tags/v1.92.2b-docking.zip
        imgui-docking.zip
    )
    execute_process(COMMAND 
        tar -xzvf imgui-docking.zip -C ${CMAKE_CURRENT_LIST_DIR}
    )
    file(GLOB IMGUI_DOCKING_PATHS ${CMAKE_CURRENT_LIST_DIR}/imgui-*-docking)
    list(FILTER IMGUI_DOCKING_PATHS EXCLUDE REGEX "\\.cmake$")
    list(GET IMGUI_DOCKING_PATHS 0 IMGUI_DOCKING_PATH)
endif()

file(GLOB CPP_FILES 
    ${IMGUI_DOCKING_PATH}/*.cpp
)

file(GLOB BACKENDS_FILES 
    ${IMGUI_DOCKING_PATH}/backends/imgui_impl_win32.cpp
    ${IMGUI_DOCKING_PATH}/backends/imgui_impl_dx11.cpp
)

add_library(${PROJECT_NAME} STATIC 
    ${CPP_FILES} 
    ${BACKENDS_FILES}
)
add_library(Mortis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} PUBLIC 
    ${IMGUI_DOCKING_PATH}
    ${IMGUI_DOCKING_PATH}/backends
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    d3d11 
	dxgi
)