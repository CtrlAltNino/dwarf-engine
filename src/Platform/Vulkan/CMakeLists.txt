IF(WIN32 OR(UNIX AND NOT APPLE))
    target_sources(${targetName}
        PRIVATE
        VulkanRendererApi.cpp
    )
ENDIF()