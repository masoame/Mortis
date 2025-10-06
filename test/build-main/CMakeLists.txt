
project(main LANGUAGES CXX)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)

target_link_libraries(${PROJECT_NAME} 
	Mortis::Lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES 
   WIN32_EXECUTABLE FALSE
)


#target_compile_options(main PRIVATE
#    $<$<COMPILE_LANGUAGE:CXX>:/W0 /WX->
#    $<$<COMPILE_LANGUAGE:C>:/W0 /WX->
#)