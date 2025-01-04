# Set the minimum required CMake Version
cmake_minimum_required(VERSION 3.22)

# Helper functions
include(${CMAKE_SOURCE_DIR}/helper.cmake)
set(DNFD_PORTAL ON CACHE BOOL "Use portal for NFD")

# add_compile_definitions(NOMINMAX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(libname dwarf-lib)
set(targetName dwarf-engine)
set(testTarget dwarf-test)

set(CMAKE_PCH_INSTANTIATE_TEMPLATES ON)
set(VCPKG_MAX_CONCURRENCY 8)

# Platform dependant settings
IF(WIN32)
    file(READ "$ENV{LOCALAPPDATA}/vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")

    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon_windows.rc")
    set(EXECUTABLE_ADDITIONAL_SETTINGS ${APP_ICON_RESOURCE_WINDOWS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Hiding the console window in release mode
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # using Visual Studio C++
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    endif()

    # Maybe move to windows only, need to test
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
ELSEIF(UNIX AND NOT APPLE)
    # ~/.vcpkg/vcpkg.path.txt
    file(READ "$ENV{HOME}/.vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-linux")

ELSEIF(APPLE)
    # ~/.vcpkg/vcpkg.path.txt
    file(READ "$ENV{HOME}/.vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-osx")
    enable_language(OBJC OBJCXX)
ENDIF()

enable_testing()

# Project declaration
project(Dwarf\ Engine VERSION 0.0.1 LANGUAGES C CXX)

add_executable(${targetName} "")
include_directories(src)
add_subdirectory(src)
target_sources(${targetName} PRIVATE ${EXECUTABLE_ADDITIONAL_SETTINGS})
target_link_libraries(${targetName} PRIVATE ${libname})

add_subdirectory(tests)

# Copy files to build folder
add_custom_command(TARGET
    ${targetName}
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${targetName}>/data)

add_custom_command(TARGET
    ${targetName}
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${targetName}>/data)