#pragma once

#include "../../utilities/dpch.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"IRenderTexture.h"

class RenderTextureOpenGL : public IRenderTexture{
    private:
        GLuint fbo;
        GLuint textureColorBuffer;
        GLuint rbo;
    public:
        RenderTextureOpenGL();
        virtual int GetID();
        virtual int GetTexture();
        virtual void Bind();
        virtual void Unbind();
        virtual void UpdateTextureResolution();
};