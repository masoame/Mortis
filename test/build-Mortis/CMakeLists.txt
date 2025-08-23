project(Mortis)
cmake_policy(SET CMP0167 NEW)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME}
	Mortis::imgui
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
   WIN32_EXECUTABLE FALSE
)