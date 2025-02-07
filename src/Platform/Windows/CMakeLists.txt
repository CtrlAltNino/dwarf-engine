IF(WIN32)
    target_sources(${libname}
        PRIVATE
        WindowsWindow.cpp
    )
ENDIF()