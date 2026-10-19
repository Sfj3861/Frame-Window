#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(FrameWindowMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                "Frame Window",
                90,
                true,
                "bigFont.fnt",
                "GJ_button_01.png",
                30.0f,
                0.8f
            ),
            this,
            menu_selector(FrameWindowMenuLayer::onFrameWindow)
        );

        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({winSize.width - 65.0f, 45.0f});
        this->addChild(menu, 10);

        return true;
    }

    void onFrameWindow(CCObject*) {
        auto mod = Mod::get();

        int frames = mod->getSettingValue<int>("window-frames");
        bool overlay = mod->getSettingValue<bool>("show-overlay");
        bool sound = mod->getSettingValue<bool>("play-click-sound");

        auto message = fmt::format(
            "Practice settings\n\nTiming window: +/- {} frames\nOverlay: {}\nClick feedback: {}\n\nThis starter build displays settings only; it does not modify inputs, collisions, or player physics.",
            frames,
            overlay ? "ON" : "OFF",
            sound ? "ON" : "OFF"
        );

        FLAlertLayer::create("Frame Window v0.1.0", message, "OK")->show();
    }
};
