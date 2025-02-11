IF(WIN32)
    target_sources(${libname}
        PRIVATE
        AmdGpuInfo.cpp
    )
ENDIF()