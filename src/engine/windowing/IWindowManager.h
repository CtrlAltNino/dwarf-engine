#pragma once

#include "../../utilities/dpch.h"

#include<imgui_internal.h>
#include<glm/vec4.hpp>

#include"../../utilities/Common.h"
#include"../rendering/IRenderTexture.h"

#define INITIAL_WINDOW_WIDTH (1280)
#define INITIAL_WINDOW_HEIGHT (720)
#define INTER_REGULAR_PATH "data/engine/fonts/Roboto-Regular.ttf"
#define INTER_BOLD_PATH "data/engine/fonts/Inter-Bold.ttf"

class IWindowManager {
    protected:
        /// @brief Clear color for the framebuffer of the window.
        glm::vec4 clearColor = glm::vec4(0.1,0.1,0.1,1);

        /// @brief Current size of the window.
        glm::ivec2 windowSize;

        /// @brief Render textures maintained by this window.
        std::vector<IRenderTexture*> renderTextures;
    public:
        /// @brief Key: Name of the font. Value: Loaded fonts used in IMGUI.
        static inline std::map<std::string,ImFont*> fonts;

        /// @brief Saved name of the render device.
        static inline std::string rendererName;

        /// @brief Saved vendor name of the context.
        static inline std::string vendorName;

        /// @brief Saved version of the context API.
        static inline std::string apiVersion;

        /// @brief Initializes the window manager.
        virtual void Init()=0;

        /// @brief Returns the used API.
        /// @return The used API represented as an enum.
        virtual GraphicsApi GetActiveApi()=0;

        /// @brief Creates the window of the Dwarf Engine editor.
        virtual void CreateEditorWindow()=0;

        /// @brief Changes the title of the window.
        /// @param windowName The new title.
        virtual void SetWindowTitle(std::string windowName)=0;

        /// @brief Executing all window related tasks needed to be done in the beginning of a frame.
        virtual void StartFrame()=0;
        
        /// @brief Executing all window related tasks needed to be done at the end of a frame.
        virtual void EndFrame()=0;

        /// @brief Returns the size of the window.
        /// @return Integer vector containing the size of the window.
        virtual glm::ivec2 GetWindowSize()=0;

        /// @brief Returns if the window should be closed.
        /// @return True if the window should be closed, false if not.
        virtual bool ShouldWindowCloseSignal()=0;

        /// @brief Executes all initializing IMGUI tasks.
        virtual void InitImGui()=0;

        /// @brief Adds a new render texture.
        /// @return Pointer to the created render texture.
        virtual IRenderTexture* AddRenderTexture()=0;

        /// @brief Removes the render texture with the given ID.
        /// @param ID ID of the render texture to remove.
        virtual void RemoveRenderTexture(int ID)=0;

        /// @brief Returns the render textures maintained by the window.
        /// @return Pointer to a vector containing the render textures.
        virtual std::vector<IRenderTexture*>* GetRenderTextures() {return &renderTextures;}
};