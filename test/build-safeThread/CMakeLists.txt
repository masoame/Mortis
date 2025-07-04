project(safeThread)

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp )
set_target_properties(${PROJECT_NAME} PROPERTIES 
   WIN32_EXECUTABLE FALSE
)