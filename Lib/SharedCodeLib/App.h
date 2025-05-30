#pragma once

#include "ViewDelegate.h"

class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    ~MyAppDelegate() override;

    NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification* notification) override;
    void applicationDidFinishLaunching(NS::Notification* notification) override;
    bool applicationShouldTerminateAfterLastWindowClosed(
        NS::Application* sender) override;

private:
    NS::Window* window;
    MTK::View* mtkView;
    MTL::Device* device;

    std::unique_ptr<MyMTKViewDelegate> delegate;
};
