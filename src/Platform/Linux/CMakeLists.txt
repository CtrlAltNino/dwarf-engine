IF(UNIX AND NOT APPLE)
    target_sources(${targetName}
        PRIVATE
        LinuxWindow.cpp
    )
ENDIF()