IF(WIN32 OR(UNIX AND NOT APPLE))
    target_sources(${libname}
        PRIVATE
        OpenGLMesh.cpp
        OpenGLContext.cpp
        OpenGLImGuiLayer.cpp
        OpenGLFramebuffer.cpp
        OpenGLShader.cpp
        OpenGLTexture.cpp
        OpenGLRendererApi.cpp
        OpenGLComputeShader.cpp
        OpenGLStateTracker.cpp
        OpenGLShaderAssetSelector.cpp
    )
ENDIF()