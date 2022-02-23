# Set the minimum required CMake Version
cmake_minimum_required(VERSION 3.22)

# Helper functions
include(${CMAKE_SOURCE_DIR}/helper.cmake)

# Maybe move to windows only, need to test
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

# Need to find a way to make the path to the vcpkg.cmake file dynamic, maybe I can even store it in the repo

# Declaring a variable that can contain various settings for the executable
set(EXECUTABLE_ADDITIONAL_SETTINGS "")

# Platform dependant settings
IF (WIN32)
    set(CMAKE_TOOLCHAIN_FILE "E:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon_windows.rc")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(EXECUTABLE_ADDITIONAL_SETTINGS ${APP_ICON_RESOURCE_WINDOWS})
    
    # Hiding the console window in release mode
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    
    add_executable(s3de "")
ELSEIF (APPLE)
    set(VCPKG_TARGET_TRIPLET "x64-osx-static")
    add_executable(s3de "")
ELSEIF (UNIX AND NOT APPLE)
    set(CMAKE_TOOLCHAIN_FILE "~/Documents/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-linux")
    add_executable(s3de "")
ENDIF()

target_sources(s3de PRIVATE ${EXECUTABLE_ADDITIONAL_SETTINGS})

# Project declaration
project(Simple3DEngine VERSION 0.0.1)
set(CXX_STANDARD 17)

# Add the CMakeLists.txt files of the subdirectories
smtg_add_subdirectories()

# Linking dependencies
find_package(glad CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE glad::glad)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(s3de PRIVATE ${STB_INCLUDE_DIRS})

find_package(assimp CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE assimp::assimp)

find_path(BEXT_DI_INCLUDE_DIRS "boost/di.hpp")
target_include_directories(s3de PRIVATE ${BEXT_DI_INCLUDE_DIRS})

find_package(glad CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE glm::glm)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE imgui::imgui)

find_package(unofficial-nativefiledialog CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE unofficial::nativefiledialog::nfd)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(s3de PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

# Copy files to build folder
add_custom_command(TARGET 
 s3de 
 POST_BUILD COMMAND ${CMAKE_COMMAND} 
 -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:s3de>/data)