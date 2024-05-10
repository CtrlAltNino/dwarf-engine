# Set the minimum required CMake Version
cmake_minimum_required(VERSION 3.22)

# Helper functions
include(${CMAKE_SOURCE_DIR}/helper.cmake)

# add_compile_definitions(NOMINMAX)
set(CMAKE_CXX_STANDARD 20)

set(targetName dwarf-engine)
set(testTarget dwarf-test)

set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)

# Need to find a way to make the path to the vcpkg.cmake file dynamic, maybe I can even store it in the repo

# Declaring a variable that can contain various settings for the executable
set(EXECUTABLE_ADDITIONAL_SETTINGS "")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# using Clang
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
# using GCC
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
# using Intel C++
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
endif()

# Platform dependant settings
IF(WIN32)
    file(READ "$ENV{LOCALAPPDATA}/vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon_windows.rc")
    set(EXECUTABLE_ADDITIONAL_SETTINGS ${APP_ICON_RESOURCE_WINDOWS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Hiding the console window in release mode
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # using Visual Studio C++
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()

    # Maybe move to windows only, need to test
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)

    add_executable(${targetName} "")
ELSEIF(UNIX AND NOT APPLE)
    # ~/.vcpkg/vcpkg.path.txt
    file(READ "$ENV{HOME}/.vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
    add_executable(${targetName} "")
ELSEIF(APPLE)
    # ~/.vcpkg/vcpkg.path.txt
    file(READ "$ENV{HOME}/.vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-osx")
    add_executable(${targetName} "")
    enable_language(OBJC OBJCXX)
ENDIF()

include_directories(src)

target_sources(${targetName} PRIVATE ${EXECUTABLE_ADDITIONAL_SETTINGS})

# Project declaration
project(Dwarf\ Engine VERSION 0.0.1 LANGUAGES C CXX)

# Add the CMakeLists.txt files of the subdirectories
smtg_add_subdirectories()

# Linking dependencies
IF(WIN32 OR(UNIX AND NOT APPLE))
    find_package(glad CONFIG REQUIRED)
    target_link_libraries(${targetName} PRIVATE glad::glad)
ENDIF()

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
target_include_directories(${targetName} PRIVATE ${STB_INCLUDE_DIRS})

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE assimp::assimp)

find_path(BEXT_DI_INCLUDE_DIRS "boost/di.hpp")
target_include_directories(${targetName} PRIVATE ${BEXT_DI_INCLUDE_DIRS})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2::SDL2-static)

# find_package(glfw3 CONFIG REQUIRED)
# target_link_libraries(${targetName} PRIVATE glfw)
find_package(glm CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE glm::glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE imgui::imgui)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE unofficial::nativefiledialog::nfd)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(efsw CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE efsw::efsw)

find_package(Threads REQUIRED)
target_link_libraries(${targetName} PRIVATE Threads::Threads)

find_package(imguizmo CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE imguizmo::imguizmo)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE EnTT::EnTT)

find_package(xxHash CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE xxHash::xxhash)

find_package(platform_folders CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE sago::platform_folders)

find_package(libjpeg-turbo CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE $<IF:$<TARGET_EXISTS:libjpeg-turbo::turbojpeg>,libjpeg-turbo::turbojpeg,libjpeg-turbo::turbojpeg-static>)

find_package(SPNG CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE $<IF:$<TARGET_EXISTS:spng::spng>,spng::spng,spng::spng_static>)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${targetName} PRIVATE spdlog::spdlog)

IF(APPLE)
    find_library(METAL Metal)
    find_library(FOUNDATION Foundation)
    find_library(QUARTZCORE QuartzCore)

    target_link_libraries(${targetName} PRIVATE ${METAL})
    target_link_libraries(${targetName} PRIVATE ${FOUNDATION})
    target_link_libraries(${targetName} PRIVATE ${QUARTZCORE})

    # target_include_directories(${targetName} PRIVATE "lib/metal-cpp/")
    target_include_directories(${targetName} PRIVATE ${CMAKE_SOURCE_DIR}/include/metal-cpp)
ENDIF()

# Copy files to build folder
add_custom_command(TARGET
    ${targetName}
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${targetName}>/data)