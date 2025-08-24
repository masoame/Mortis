project(imgui)

file(GLOB IMGUI_PATH ${CMAKE_BINARY_DIR}/_deps/imgui-*-docking)

if(NOT IMGUI_PATH)
    file(DOWNLOAD 
        https://github.com/ocornut/imgui/archive/refs/tags/v1.92.2b-docking.zip
        imgui-docking.zip
    )
    execute_process(COMMAND 
        tar -xzvf imgui-docking.zip -C ${CMAKE_BINARY_DIR}/_deps
    )
    file(GLOB IMGUI_PATH ${CMAKE_BINARY_DIR}/_deps/imgui-*-docking)
endif()

file(GLOB CPP_FILES 
    ${IMGUI_PATH}/*.cpp
)

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