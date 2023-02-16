IF (APPLE)
    target_sources(${targetName}
        PRIVATE
            WindowManagerMetal.cpp
    )
ENDIF()

IF (WIN32)
    target_sources(${targetName}
        PRIVATE
            WindowManagerOpenGL.cpp
    )
ENDIF()