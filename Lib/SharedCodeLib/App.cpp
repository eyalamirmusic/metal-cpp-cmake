#include "App.h"
#include "AutoReleasePool.h"

namespace Apple
{

static void onQuit(void*, SEL, const NS::Object* pSender)
{
    auto app = NS::Application::sharedApplication();
    app->terminate(pSender);
}

static void windowClose(void*, SEL, const NS::Object*)
{
    auto app = NS::Application::sharedApplication();
    app->windows()->object<NS::Window>(0)->close();
}

NS::Menu* MyAppDelegate::createMenuBar()
{
    using NS::StringEncoding::UTF8StringEncoding;

    auto mainMenu = NS::Menu::alloc()->init();
    auto appMenuItem = NS::MenuItem::alloc()->init();
    auto appMenu =
        NS::Menu::alloc()->init(NS::String::string("Appname", UTF8StringEncoding));

    auto appName = NS::RunningApplication::currentApplication()->localizedName();
    auto quitItemName = NS::String::string("Quit ", UTF8StringEncoding)
                            ->stringByAppendingString(appName);
    SEL quitCb = NS::MenuItem::registerActionCallback("appQuit", onQuit);

    auto pAppQuitItem = appMenu->addItem(
        quitItemName, quitCb, NS::String::string("q", UTF8StringEncoding));
    pAppQuitItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);
    appMenuItem->setSubmenu(appMenu);

    auto windowMenuItem = NS::MenuItem::alloc()->init();
    auto windowMenu =
        NS::Menu::alloc()->init(NS::String::string("Window", UTF8StringEncoding));

    SEL closeWindowCb =
        NS::MenuItem::registerActionCallback("windowClose", windowClose);
    auto closeMenuItem =
        windowMenu->addItem(NS::String::string("Close Window", UTF8StringEncoding),
                            closeWindowCb,
                            NS::String::string("w", UTF8StringEncoding));
    closeMenuItem->setKeyEquivalentModifierMask(NS::EventModifierFlagCommand);

    windowMenuItem->setSubmenu(windowMenu);

    mainMenu->addItem(appMenuItem);
    mainMenu->addItem(windowMenuItem);

    appMenuItem->release();
    windowMenuItem->release();
    appMenu->release();
    windowMenu->release();

    return mainMenu->autorelease();
}

void MyAppDelegate::applicationWillFinishLaunching(NS::Notification* notification)
{
    auto menu = createMenuBar();
    auto app = reinterpret_cast<NS::Application*>(notification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void MyAppDelegate::applicationDidFinishLaunching(NS::Notification* notification)
{
    auto frame = (CGRect) {{100.0, 100.0}, {512.0, 512.0}};

    window = NS::TransferPtr(NS::Window::alloc()->init(
        frame,
        NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
        NS::BackingStoreBuffered,
        false));

    device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    renderer.setDevice(device);

    mtkView = NS::TransferPtr(MTK::View::alloc()->init(frame, device.get()));
    mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
    mtkView->setClearColor(MTL::ClearColor::Make(1.0, 0.0, 0.0, 1.0));

    delegate = std::make_unique<MyMTKViewDelegate>(renderer);
    mtkView->setDelegate(delegate.get());

    window->setContentView(mtkView.get());
    window->setTitle(
        NS::String::string("00 - Window", NS::StringEncoding::UTF8StringEncoding));

    window->makeKeyAndOrderFront(nullptr);

    auto app = reinterpret_cast<NS::Application*>(notification->object());
    app->activateIgnoringOtherApps(true);
}

bool MyAppDelegate::applicationShouldTerminateAfterLastWindowClosed(
    NS::Application* sender)
{
    return true;
}

void startAppWith(Renderer& renderer)
{
    auto releasePool = Apple::AutoReleasePool();
    auto del = Apple::MyAppDelegate(renderer);

    auto* app = NS::Application::sharedApplication();
    app->setDelegate(&del);
    app->run();
}
} // namespace Apple