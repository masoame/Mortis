project(MortisLib)

add_library(MortisLib INTERFACE)

target_link_libraries(MortisLib INTERFACE
	Mortis::core::Utils
	Mortis::core::PE
	Mortis::core::SysIntVecDbg
)



