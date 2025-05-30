#include "ViewDelegate.h"

MyMTKViewDelegate::MyMTKViewDelegate(MTL::Device* device)
    : ViewDelegate()
    , renderer(device)
{
}

void MyMTKViewDelegate::drawInMTKView(MTK::View* view)
{
    renderer.draw(view);
}
