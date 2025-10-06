project(DLLTest)
cmake_policy(SET CMP0167 NEW)

add_library(${PROJECT_NAME} SHARED "${PROJECT_NAME}.cpp")
target_link_libraries(${PROJECT_NAME}
	Mortis::DLLMaker
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
   WIN32_EXECUTABLE FALSE
)