smtg_add_subdirectories()

target_sources(${targetName}
    PRIVATE
        #EBO.cpp
        Material.cpp
        Mesh.cpp
        Shader.cpp
        #VAO.cpp
        #VBO.cpp
        Renderer.cpp
        GraphicsContext.cpp
        Framebuffer.cpp
        Texture.cpp
        RendererApi.cpp
)