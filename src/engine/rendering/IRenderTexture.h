#pragma once

#include<glm/vec2.hpp>
#include"../scene/Camera.h"

enum class RENDER_MODE {FREE, RESOLUTION, ASPECT_RATIO};

/// @brief Base render texture class.
class IRenderTexture {
    protected:
        /// @brief Texture width.
        int textureWidth = 512;

        /// @brief Texture height.
        int textureHeight = 512;

        /// @brief Aspect ratio of the render texture.
        float aspectRatio = 1;

        /// @brief Defines the constrain mode of the render texture.
        RENDER_MODE renderMode = RENDER_MODE::FREE;

        /// @brief Pointer to the camera of this render texture.
        Camera* camera;
    public:
        /// @brief Binds this render texture.
        virtual void Bind()=0;

        /// @brief Unbinds the render texture.
        virtual void Unbind()=0;

        /// @brief Returns the handle of the texture.
        /// @return The ID handle of the render texture.
        virtual int GetID()=0;

        /// @brief Returns the texture itself.
        /// @return Pointer the texture.
        virtual void* GetTexture()=0;

        /// @brief Assigns the camera.
        /// @param camera Pointer to a camera instance.
        void SetCamera(Camera* camera){
            this->camera = camera;
        }

        /// @brief Returns a pointer to a camera instance.
        /// @return Pointer to a camera.
        Camera* GetCamera(){
            return camera;
        }

        /// @brief Returns the render mode of this render texture.
        /// @return The render mode.
        RENDER_MODE GetRenderMode(){
            return renderMode;
        }
        
        /// @brief Returns the resolution of the render texture.
        /// @return The resolution as a 2D integer vector.
        glm::ivec2 GetResolution() {
            return glm::ivec2(textureWidth, textureHeight);
        }
        
        /// @brief Returns the aspect ratio of the render texture.
        /// @return The aspect ratio of the render texture as a float.
        float GetAspectRatio() {
            return aspectRatio;
        }

        /// @brief Updates the texture resolution to the new values.
        virtual void UpdateTextureResolution()=0;
        
        /// @brief Sets the resolution values.
        /// @param width Desired width of the texture.
        /// @param height Desired height of the texture.
        void SetResolution(int width, int height){
            textureWidth = width;
            textureHeight = height;
            //UpdateTextureResolution();
        }

        /// @brief Sets the aspect ratio constraint.
        /// @param aspectRatio Desired aspect ratio as a float.
        void SetAspectRatio(float aspectRatio){
            aspectRatio = aspectRatio;
            //UpdateTextureResolution();
        }

        /// @brief Sets the render mode constraint.
        /// @param renderMode Desired render mode.
        void SetRenderMode(RENDER_MODE renderMode){
            renderMode = renderMode;
            //UpdateTextureResolution();
        }
};