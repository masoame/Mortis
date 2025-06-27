
project(Utils LANGUAGES CXX)

file(GLOB UtilsSourceFiles 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${UtilsSourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tools
)

target_link_libraries(${PROJECT_NAME} 
	spdlog
)


