project(PE LANGUAGES CXX)

file(GLOB SRC_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRC_SOURCES} "inc/tools/ReflFuncResult.h")

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tools

	${CMAKE_CURRENT_SOURCE_DIR}/inc-hpp
	${CMAKE_CURRENT_SOURCE_DIR}/inc-hpp/tools-hpp
)

target_link_libraries(${PROJECT_NAME} 
	Utils
)


