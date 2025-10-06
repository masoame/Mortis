project(DLLMaker LANGUAGES CXX)

file(GLOB SRC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SRC_SOURCES})
add_library(Mortis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(${PROJECT_NAME} 
	Mortis::PE
)


#添加编译参数UNICODE
#target_compile_options(${PROJECT_NAME} PUBLIC /DUNICODE)

