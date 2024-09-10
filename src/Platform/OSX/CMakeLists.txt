IF(APPLE)
    target_sources(${libname}
        PRIVATE
        OSXWindow.cpp
    )
ENDIF()