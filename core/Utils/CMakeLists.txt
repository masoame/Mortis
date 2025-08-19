
project(Utils LANGUAGES CXX)

file(GLOB UtilsSourceFiles 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${UtilsSourceFiles})
add_library(Mortis::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tools
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tools/ThreadSafe
)

target_precompile_headers(${PROJECT_NAME} 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/inc/utils.hpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC
	nlohmann_json
	PRIVATE 
	spdlog::spdlog
)


