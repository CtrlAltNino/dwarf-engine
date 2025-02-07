

add_library(${libname} STATIC "")

smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
    EntryPoint.cpp
)

target_precompile_headers(${libname} PUBLIC pch.h)

IF(WIN32)
    target_link_libraries(${libname} PUBLIC bcrypt)
ENDIF()

# Linking dependencies
IF(WIN32 OR(UNIX AND NOT APPLE))
    find_package(glad CONFIG REQUIRED)

    target_link_libraries(${libname} PUBLIC glad::glad)
ENDIF()

find_package(Stb REQUIRED)
target_include_directories(${libname} PUBLIC ${Stb_INCLUDE_DIR})

find_package(assimp CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC assimp::assimp)

find_path(BEXT_DI_INCLUDE_DIRS "boost/di.hpp")
target_include_directories(${libname} PUBLIC ${BEXT_DI_INCLUDE_DIRS})

find_path(BOOST_INCLUDE_DIRS "boost/")
target_include_directories(${libname} PUBLIC ${BOOST_INCLUDE_DIRS})

find_package(efsw CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC efsw::efsw)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC EnTT::EnTT)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC glm::glm)

find_package(SDL2 CONFIG REQUIRED)

target_link_libraries(${libname}
    PUBLIC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC imgui::imgui)

find_package(unofficial-imguizmo CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC unofficial::imguizmo::imguizmo)

find_package(libjpeg-turbo CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC libjpeg-turbo::turbojpeg-static)

find_package(SPNG CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC $<IF:$<TARGET_EXISTS:spng::spng>,spng::spng,spng::spng_static>)

find_package(nfd CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC nfd::nfd)
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC nlohmann_json::nlohmann_json)

find_package(platform_folders CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC sago::platform_folders)

find_package(Protobuf CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC spdlog::spdlog)

find_package(freeimage CONFIG REQUIRED)
target_link_libraries(${libname} PRIVATE freeimage::FreeImage freeimage::FreeImagePlus)

find_package(TIFF REQUIRED)
target_link_libraries(${libname} PUBLIC TIFF::TIFF)

find_package(xxHash CONFIG REQUIRED)
target_link_libraries(${libname} PUBLIC xxHash::xxhash)

find_package(Threads REQUIRED)
target_link_libraries(${libname} PUBLIC Threads::Threads)

IF(APPLE)
    find_library(METAL Metal)
    find_library(FOUNDATION Foundation)
    find_library(QUARTZCORE QuartzCore)

    target_link_libraries(${libname} PUBLIC ${METAL})
    target_link_libraries(${libname} PUBLIC ${FOUNDATION})
    target_link_libraries(${libname} PUBLIC ${QUARTZCORE})

    # target_include_directories(${libname} PRIVATE "lib/metal-cpp/")
    target_include_directories(${libname} PUBLIC ${CMAKE_SOURCE_DIR}/include/metal-cpp)
ENDIF()