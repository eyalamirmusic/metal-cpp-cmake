#pragma once

#include "ViewDelegate.h"
#include "Renderer.h"
#include "AutoReleasePool.h"

namespace Apple
{
class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    MyAppDelegate(Renderer& rendererToUse)
        : renderer(rendererToUse)
    {

    }

    ~MyAppDelegate() override;

    NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(
        NS::Application* sender) override;

private:
    NS::Window* window = nullptr;
    MTK::View* mtkView = nullptr;
    MTL::Device* device = nullptr;

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