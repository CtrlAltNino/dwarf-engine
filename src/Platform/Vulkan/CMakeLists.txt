IF(WIN32 OR(UNIX AND NOT APPLE))
    target_sources(${libname}
        PRIVATE
        VulkanRendererApi.cpp
    )
ENDIF()