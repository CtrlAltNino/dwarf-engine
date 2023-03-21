IF (APPLE)
    target_sources(${targetName}
        PRIVATE
            WindowManagerMetal.cpp
            MetalTexture.cpp
    )
ENDIF()