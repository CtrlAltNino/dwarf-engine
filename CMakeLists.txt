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

add_compile_definitions(BOOST_DI_CFG_CTOR_LIMIT_SIZE=20)

# Platform dependant settings
IF(WIN32)
    file(READ "$ENV{LOCALAPPDATA}/vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-windows-static")
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/app_icon_windows.rc")
    set(EXECUTABLE_ADDITIONAL_SETTINGS ${APP_ICON_RESOURCE_WINDOWS})
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Project declaration
    project(Dwarf\ Engine VERSION 0.0.1 LANGUAGES C CXX)

    enable_testing()

    add_executable(${targetName} WIN32 "")

    # Hiding the console window in release mode
    target_link_options(${targetName} PRIVATE
        $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
        $<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>)

    # Maybe move to windows only, need to test
    ADD_DEFINITIONS(-DUNICODE)
    ADD_DEFINITIONS(-D_UNICODE)
ELSEIF(UNIX)
    # ~/.vcpkg/vcpkg.path.txt
    file(READ "$ENV{HOME}/.vcpkg/vcpkg.path.txt" VCPKG_PATH)
    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_PATH}/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_TARGET_TRIPLET "x64-linux")

    # Project declaration
    project(Dwarf\ Engine VERSION 0.0.1 LANGUAGES C CXX)

    enable_testing()

    add_executable(${targetName} "")
ENDIF()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument")
endif()

include_directories(src)
include_directories(include)
include_directories(tests)
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