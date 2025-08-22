project(MortisLib)

add_library(MortisLib INTERFACE)
add_library(Mortis::Lib ALIAS MortisLib)

target_link_libraries(MortisLib INTERFACE
	Mortis::Utils
	Mortis::PE
	Mortis::SysIntVecDbg
)