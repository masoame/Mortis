
project(winmain LANGUAGES CXX)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} 
	imgui
)
set_target_properties(${PROJECT_NAME} PROPERTIES 
   WIN32_EXECUTABLE FALSE
)