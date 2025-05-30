#pragma once


#include "Renderer.h"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate(MTL::Device* device);

    void drawInMTKView(MTK::View* view) override;

private:
    Renderer renderer;
};
