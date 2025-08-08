
project(Utils LANGUAGES CXX)

file(GLOB UtilsSourceFiles 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp
)

find_package(Boost ${BOOST_VERSION} REQUIRED COMPONENTS program_options)

add_library(${PROJECT_NAME} STATIC ${UtilsSourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	${CMAKE_CURRENT_SOURCE_DIR}/inc/tools
)

target_precompile_headers(${PROJECT_NAME} 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/inc/utils.hpp
)

target_link_libraries(${PROJECT_NAME} 
	spdlog
	#Boost::boost
)


