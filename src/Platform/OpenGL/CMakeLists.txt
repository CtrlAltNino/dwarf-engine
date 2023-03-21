IF (WIN32)
    target_sources(${targetName}
        PRIVATE
            OpenGLContext.cpp
            OpenGLImGuiLayer.cpp
            OpenGLFramebuffer.cpp
            OpenGLShader.cpp
            OpenGLTexture.cpp
    )
ENDIF()