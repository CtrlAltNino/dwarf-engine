#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"IRenderTexture.h"

/// @brief OpenGL implementation of a render texture.
class RenderTextureOpenGL : public IRenderTexture{
    private:
        /// @brief Handle of the frame buffer.
        GLuint fbo;

        /// @brief Handle of the color buffer.
        GLuint textureColorBuffer;

        /// @brief Handle of the render buffer.
        GLuint rbo;
    public:
        RenderTextureOpenGL();

        /// @brief Returns the handle of the texture.
        /// @return The ID handle of the render texture.
        virtual int GetID();

        /// @brief Returns the texture itself.
        /// @return Pointer the texture.
        virtual void* GetTexture();

        /// @brief Binds this render texture.
        virtual void Bind();

        /// @brief Unbinds the render texture.
        virtual void Unbind();

        /// @brief Updates the texture resolution to the new values.
        virtual void UpdateTextureResolution();
};