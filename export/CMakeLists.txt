project(MortisLib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Mortis::Lib ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE
	Mortis::Utils
	Mortis::PE
	Mortis::SysIntVecDbg
	Mortis::imgui
)

project(MortisDLLLib)

add_library(${PROJECT_NAME} INTERFACE)
add_library(Mortis::DLL::Lib ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE
	Mortis::Utils
	Mortis::PE
	Mortis::SysIntVecDbg
	Mortis::imgui
	Mortis::DLLMaker
)