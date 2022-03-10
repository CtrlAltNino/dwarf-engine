#pragma once

#include"IRenderTexture.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>

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