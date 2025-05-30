#pragma once


#include "Renderer.h"

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate(MTL::Device* pDevice);

    ~MyMTKViewDelegate() override;
    void drawInMTKView(MTK::View* pView) override;

private:
    Renderer* _pRenderer;
};
