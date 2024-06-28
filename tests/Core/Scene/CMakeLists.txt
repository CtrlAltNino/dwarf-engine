smtg_add_subdirectories()

set(TEST_SOURCES ${TEST_SOURCES} PARENT_SCOPE
    ${CMAKE_CURRENT_SOURCE_DIR}/CameraTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/EntityTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneTests.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SceneUtilitiesTests.cpp
)