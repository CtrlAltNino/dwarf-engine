smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
        EBO.cpp
        Material.cpp
        Mesh.cpp
        #RenderTextureOpenGL.cpp
        Shader.cpp
        Texture.cpp
        VAO.cpp
        VBO.cpp
        Renderer.cpp
        GraphicsContext.cpp
        Framebuffer.cpp
)