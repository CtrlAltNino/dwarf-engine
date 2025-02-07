IF(WIN32)
    target_sources(${libname}
        PRIVATE
        D3D12RendererApi.cpp
    )
ENDIF()