IF(UNIX AND NOT APPLE)
    target_sources(${libname}
        PRIVATE
        LinuxWindow.cpp
    )
ENDIF()