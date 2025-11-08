set(ENV{http_proxy} "http://127.0.0.1:10808")
set(ENV{https_proxy} "http://127.0.0.1:10808")

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG  v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.3
    CMAKE_ARGS     -DSPDLOG_FMT_EXTERNAL=ON
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY  https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7
)
FetchContent_MakeAvailable(magic_enum)

FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY  https://github.com/cameron314/concurrentqueue.git
    GIT_TAG v1.0.4
)
FetchContent_MakeAvailable(concurrentqueue)

FetchContent_Declare(
    sigslot
    GIT_REPOSITORY  https://github.com/palacaze/sigslot.git
    GIT_TAG v1.2.3
)
FetchContent_MakeAvailable(sigslot)

FetchContent_Declare(
    reflect_cpp
    GIT_REPOSITORY  https://github.com/getml/reflect-cpp.git
    GIT_TAG v0.22.0
)
FetchContent_MakeAvailable(reflect_cpp)


if(NOT USING_IMGUI)
    set(USING_IMGUI on)
endif()

if(NOT IMGUI_DOCKING_OPTION)
    set(IMGUI_DOCKING_OPTION off)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/thirdparty/import_imgui.cmake)
