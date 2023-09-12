IF (WIN32 OR (UNIX AND NOT APPLE))
target_sources(${targetName}
    PRIVATE
        OpenGLMesh.cpp
        OpenGLContext.cpp
        OpenGLImGuiLayer.cpp
        OpenGLFramebuffer.cpp
        OpenGLShader.cpp
        OpenGLTexture.cpp
        OpenGLRendererApi.cpp
)
ENDIF()