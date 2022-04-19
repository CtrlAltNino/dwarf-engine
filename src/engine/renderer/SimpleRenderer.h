#pragma once

#include "IRenderer.h"

class SimpleRenderer : public IRenderer {
    // Setting the target to render to
    virtual void SetRenderBuffer();

    // Change camera matrix
    virtual void SetCamera();

    // Submit Geometry to Render
    virtual void Submit();

    // Render
    virtual void Render();
};