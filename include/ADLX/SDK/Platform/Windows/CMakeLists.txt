IF(WIN32)
    target_sources(${libname}
        PRIVATE
        WinAPIs.cpp
    )
ENDIF()