#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr char const* kOverlayID = "frame-window-practice-overlay";

    bool overlayEnabled() {
        return Mod::get()->getSettingValue<bool>("show-overlay");
    }

    int timingWindow() {
        return Mod::get()->getSettingValue<int>("window-frames");
    }

    bool showDetails() {
        return Mod::get()->getSettingValue<bool>("show-details");
    }
}

class $modify(FrameWindowMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create(
                "Frame Window", 90, true, "bigFont.fnt",
                "GJ_button_01.png", 30.0f, 0.8f
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
        int frames = timingWindow();
        bool overlay = overlayEnabled();
        bool details = showDetails();

        auto message = fmt::format(
            "PRACTICE CONFIGURATION\n\n"
            "Timing window: +/- {} frames\n"
            "In-level overlay: {}\n"
            "Extra stats: {}\n\n"
            "The overlay is visual only. It does not change clicks, hitboxes, "
            "collisions, physics, or level completion.",
            frames,
            overlay ? "ON" : "OFF",
            details ? "ON" : "OFF"
        );
        FLAlertLayer::create("Frame Window v0.2.0", message, "OK")->show();
    }
};

class $modify(FrameWindowPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* overlay = nullptr;
        float elapsed = 0.f;
        float refreshElapsed = 0.f;
        unsigned long long renderedFrames = 0;
    };

    void onEnter() {
        PlayLayer::onEnter();

        if (!overlayEnabled())
            return;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto label = CCLabelBMFont::create("", "bigFont.fnt");
        if (!label)
            return;

        label->setID(kOverlayID);
        label->setAnchorPoint({0.f, 1.f});
        label->setPosition({8.f, winSize.height - 8.f});
        label->setScale(0.38f);
        label->setOpacity(210);
        this->addChild(label, 1000);
        m_fields->overlay = label;
        updateOverlay(0.f);
    }

    void update(float dt) {
        PlayLayer::update(dt);
        if (!m_fields->overlay || !overlayEnabled())
            return;

        m_fields->elapsed += dt;
        m_fields->refreshElapsed += dt;
        ++m_fields->renderedFrames;

        // Refresh text at about 10 Hz to keep the overlay lightweight.
        if (m_fields->refreshElapsed >= 0.1f) {
            updateOverlay(m_fields->refreshElapsed);
            m_fields->refreshElapsed = 0.f;
        }
    }

    void updateOverlay(float) {
        if (!m_fields->overlay)
            return;

        auto director = CCDirector::sharedDirector();
        float interval = director ? director->getAnimationInterval() : 0.f;
        int fps = interval > 0.f ? static_cast<int>(0.5f + 1.f / interval) : 0;
        auto text = fmt::format(
            "FRAME WINDOW  |  +/- {} FRAMES\nFPS TARGET: {}",
            timingWindow(),
            fps > 0 ? std::to_string(fps) : std::string("N/A")
        );

        if (showDetails()) {
            auto percent = this->getCurrentPercent();
            text += fmt::format("\nATTEMPT PROGRESS: {}%", percent);
            text += fmt::format("\nOVERLAY FRAMES: {}", m_fields->renderedFrames);
        }
        m_fields->overlay->setString(text.c_str());
    }
};
