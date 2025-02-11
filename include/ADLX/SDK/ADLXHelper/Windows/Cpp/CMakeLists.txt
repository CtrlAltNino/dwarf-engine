IF(WIN32)
    target_sources(${libname}
        PRIVATE
        ADLXHelper.cpp
    )
ENDIF()