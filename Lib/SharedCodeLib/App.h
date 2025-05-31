#pragma once

#include "ViewDelegate.h"
#include "Renderer.h"
#include "AutoReleasePool.h"
#include "Helpers.h"

namespace Apple
{
class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    MyAppDelegate(Renderer& rendererToUse)
        : renderer(rendererToUse)
    {

    }

    NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(
        NS::Application* sender) override;

private:
    NS::SharedPtr<NS::Window> window;
    NS::SharedPtr<MTK::View> mtkView;
    NS::SharedPtr<MTL::Device> device;

    Renderer& renderer;
    std::unique_ptr<MyMTKViewDelegate> delegate;
};

void startAppWith(Renderer& renderer);

template<typename T>
void startApp()
{
    T renderer;
    startAppWith(renderer);
}
} // namespace Apple