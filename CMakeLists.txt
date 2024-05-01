cmake_minimum_required(VERSION 3.22)
project(rollback_game)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DWIN32_LEAN_AND_MEAN)

option(BUILD_WEB "build for web" OFF)

option(USE_TRACY "Use Tracy Profiler" OFF)

option(BUILD_RELEASE "build in release" ON)

set(PHOTON_APP_ID "your_app_id_here" CACHE STRING "Set the photon app id")
add_definitions("-DPHOTON_APP_ID=\"${PHOTON_APP_ID}\"")

if (BUILD_WEB)
    add_compile_definitions(PLATFORM_WEB)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(USE_TRACY OFF)
endif ()

if (USE_TRACY)
    add_compile_definitions(TRACY_ENABLE)
    include_directories(libs/TracyProfiler/tracy)
    add_library(tracyClient STATIC libs/TracyProfiler/TracyClient.cpp)
endif()

find_package(raylib REQUIRED)
find_package(fmt REQUIRED)

# Create the photon library.
file(GLOB_RECURSE PHOTON_SRC_FILES libs/PhotonNetwork/LoadBalancing-cpp/inc/*.h libs/PhotonNetwork/LoadBalancing-cpp/src/*.cpp)
add_library(Photon ${PHOTON_SRC_FILES})
set_target_properties(Photon PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Photon PUBLIC libs/PhotonNetwork)
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(BUILD_RELEASE)
    target_link_libraries(Photon PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/PhotonNetwork/Common-cpp/Common-cpp_vc17_release_windows_md_x64.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/PhotonNetwork/Photon-cpp/Photon-cpp_vc17_release_windows_md_x64.lib
    )
else()
    target_link_libraries(Photon PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/PhotonNetwork/Common-cpp/Common-cpp_vc17_debug_windows_md_x64.lib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/PhotonNetwork/Photon-cpp/Photon-cpp_vc17_debug_windows_md_x64.lib
    )
endif()

set(data_dir "${CMAKE_SOURCE_DIR}/data")

set(data_files)
file(GLOB_RECURSE DATA_FILES
            "data/*.json"
            "data/*.png"
            "data/*.jpg"
            "data/*.wav"
            )
list(APPEND data_files ${DATA_FILES})

# Engine library
file(GLOB_RECURSE ENGINE_FILES engine/include/*.h engine/src/*.cpp)
add_library(Engine ${ENGINE_FILES})
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Engine PUBLIC engine/include/)
target_include_directories(Engine PUBLIC libs/Math/include/)
target_link_libraries(Engine PUBLIC)

if (USE_TRACY)
    target_compile_definitions(Engine PUBLIC TRACY_ENABLE)
    target_link_libraries(Engine PRIVATE tracyClient fmt::fmt)
endif()

# Common library
file(GLOB_RECURSE COMMON_FILES common/include/*.h common/src/*.cpp)
add_library(Common ${COMMON_FILES})
set_target_properties(Common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(Common PUBLIC common/include/)
target_include_directories(Common PUBLIC engine/include/)
target_include_directories(Common PUBLIC libs/Math/include/)
target_link_libraries(Common PUBLIC raylib Engine fmt::fmt Photon)

if (USE_TRACY)
    target_compile_definitions(Common PUBLIC TRACY_ENABLE)
    target_link_libraries(Common PRIVATE tracyClient)
endif()

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Common)

if (USE_TRACY)
    target_compile_definitions(main PUBLIC TRACY_ENABLE)
    target_link_libraries(main PRIVATE tracyClient)
endif()

if (BUILD_WEB)
    # The local resources path needs to be mapped to /data virtual path
    string(APPEND data_dir "@data")
    set_target_properties(main PROPERTIES LINK_FLAGS "--preload-file ${data_dir}")
endif ()

# Copy all of the resource files to the destination
if(BUILD_RELEASE)
    if(IS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release/data/")
        file(COPY ${data_files} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Release/data/")
    endif()
else()
    if(IS_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug/data/")
        file(COPY ${data_files} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug/data/")
    endif()
endif()


