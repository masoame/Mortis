
set(BOOST_OPTION OFF)
set(BOOST_ROOT "C:/Env/boost")
set(BOOST_VERSION "1.88.0")

set(ENV{http_proxy} "http://127.0.0.1:10808")
set(ENV{https_proxy} "http://127.0.0.1:10808")

include(FetchContent)


FetchContent_Declare(
    spdlog
    GIT_REPOSITORY  https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0
)

FetchContent_MakeAvailable(spdlog)