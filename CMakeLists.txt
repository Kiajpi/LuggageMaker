cmake_minimum_required(VERSION 3.20)
project(LuggageMaker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.10.5
)
set(CPR_BUILD_TESTS OFF)
FetchContent_MakeAvailable(cpr)

set(SOURCES
        src/main.cpp
        include/luggagemaker/core/item_repository.hpp
        src/core/item_repository.cpp
        include/luggagemaker/api/weather_client.hpp
        src/api/weather_client.cpp
        include/luggagemaker/core/luggage_packer.hpp
        src/core/luggage_packer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        nlohmann_json::nlohmann_json
        cpr::cpr
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})