project(MortisLib)

add_library(MortisLib INTERFACE)

target_link_libraries(MortisLib INTERFACE
	Utils
	PE
	SysIntVecDbg
)



