project(DLLMaker LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Mortis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(${PROJECT_NAME} INTERFACE 
	Mortis::Utils
)
#添加编译参数UNICODE
#target_compile_options(${PROJECT_NAME} PUBLIC /DUNICODE)

