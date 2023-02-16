target_sources(${targetName}
    PRIVATE
        EBO.cpp
        Material.cpp
        Mesh.cpp
        RenderTextureOpenGL.cpp
        Shader.cpp
        Texture.cpp
        VAO.cpp
        VBO.cpp
        Renderer.cpp
)

IF(APPLE)
target_sources(${targetName}
    PRIVATE
        MetalTexture.cpp
)
ENDIF()

IF(WIN32 OR (UNIX AND NOT APPLE))
target_sources(${targetName}
    PRIVATE
        OpenGLTexture.cpp
)
ENDIF()