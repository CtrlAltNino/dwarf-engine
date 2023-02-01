target_sources(${targetName}
    PRIVATE
        Color.cpp
        EBO.cpp
        Entity.cpp
        Mesh.cpp
        Quaternion.cpp
        Texture.cpp
        VAO.cpp
        VBO.cpp
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