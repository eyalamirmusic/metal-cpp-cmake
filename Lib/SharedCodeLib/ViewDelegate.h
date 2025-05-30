#pragma once

#include "Renderer.h"

namespace Apple
{

struct MyMTKViewDelegate : MTK::ViewDelegate
{
    MyMTKViewDelegate(Renderer& rendererToUse)
        : renderer(rendererToUse)
    {
    }

    void drawInMTKView(MTK::View* view) override { renderer.draw(view); }

    Renderer& renderer;
};
} // namespace Apple