
if(USING_BOOST)
    file(GLOB BOOST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/_deps/boost-*)
    if(NOT BOOST_ROOT)
        file(DOWNLOAD 
            https://github.com/boostorg/boost/releases/download/boost-1.89.0/boost-1.89.0-cmake.zip
            boost.zip
        )
        execute_process(COMMAND 
            tar -xzvf boost.zip -C ${CMAKE_CURRENT_BINARY_DIR}/_deps
        )
         file(GLOB BOOST_ROOT ${CMAKE_CURRENT_BINARY_DIR}/_deps/boost-*)
    endif()
endif()





