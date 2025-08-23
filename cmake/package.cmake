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
)

FetchContent_MakeAvailable(spdlog)

if(NOT USING_IMGUI)
    set(USING_IMGUI on)
endif()

if(NOT IMGUI_DOCKING_OPTION)
    set(IMGUI_DOCKING_OPTION off)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/manual_import/import_imgui.cmake)
