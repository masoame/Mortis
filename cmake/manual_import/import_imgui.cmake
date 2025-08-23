
if(USING_IMGUI)
	if(IMGUI_DOCKING_OPTION)
		include(${CMAKE_CURRENT_LIST_DIR}/imgui_option/imgui-docking_config.cmake)
	else()
		include(${CMAKE_CURRENT_LIST_DIR}/imgui_option/imgui_config.cmake)
	endif()
endif()
