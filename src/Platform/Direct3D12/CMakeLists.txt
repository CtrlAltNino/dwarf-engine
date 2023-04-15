IF (WIN32)
    target_sources(${targetName}
        PRIVATE
            D3D12RendererApi.cpp
    )
ENDIF()