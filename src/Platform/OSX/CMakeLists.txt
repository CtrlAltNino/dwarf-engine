IF(APPLE)
    target_sources(${targetName}
        PRIVATE
        OSXWindow.cpp
    )
ENDIF()