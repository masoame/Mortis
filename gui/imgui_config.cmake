project(imgui)

file(GLOB IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/imgui-*)

if(NOT IMGUI_PATH)
    file(DOWNLOAD 
        https://github.com/ocornut/imgui/archive/refs/tags/v1.92.2b.zip
        imgui.zip
    )
    execute_process(COMMAND 
        tar -xzvf imgui.zip -C ${CMAKE_CURRENT_LIST_DIR}
    )
    file(GLOB IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/imgui*)
endif()

file(GLOB CPP_FILES 
    ${IMGUI_PATH}/*.cpp)

file(GLOB BACKENDS_FILES 
    ${IMGUI_PATH}/backends/imgui_impl_win32.cpp
    ${IMGUI_PATH}/backends/imgui_impl_dx11.cpp
)

add_library(${PROJECT_NAME} STATIC 
    ${CPP_FILES} 
    ${BACKENDS_FILES}
)
add_library(Mortis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} PUBLIC 
    ${IMGUI_PATH}
    ${IMGUI_PATH}/backends
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    d3d11 
	dxgi
)

