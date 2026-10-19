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
        auto message = fmt::format(
            "PRACTICE CONFIGURATION\n\n"
            "Timing window: +/- {} frames\n"
            "In-level overlay: {}\n"
            "Extra stats: {}\n\n"
            "The overlay is visual only. It does not change clicks, hitboxes, "
            "collisions, physics, or level completion.",
            timingWindow(),
            overlayEnabled() ? "ON" : "OFF",
            showDetails() ? "ON" : "OFF"
        );
        FLAlertLayer::create("Frame Window v0.2.1", message, "OK")->show();
    }
};

class $modify(FrameWindowPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* overlay = nullptr;
        float refreshElapsed = 0.f;
        unsigned long long overlayFrames = 0;
        unsigned long long clickCount = 0;
        int lastButton = -1;
        bool lastPlayer1 = true;
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
        label->setOpacity(220);
        this->addChild(label, 1000);
        m_fields->overlay = label;
        updateOverlay();
    }

    void handleButton(bool down, int button, bool isPlayer1) {
        // Count actual button-down events only; never synthesize or alter input.
        if (down) {
            ++m_fields->clickCount;
            m_fields->lastButton = button;
            m_fields->lastPlayer1 = isPlayer1;
        }
        PlayLayer::handleButton(down, button, isPlayer1);
    }

    void update(float dt) {
        PlayLayer::update(dt);
        if (!m_fields->overlay || !overlayEnabled())
            return;

        m_fields->refreshElapsed += dt;
        ++m_fields->overlayFrames;

        // Refresh text at about 10 Hz to keep the overlay lightweight.
        if (m_fields->refreshElapsed >= 0.1f) {
            updateOverlay();
            m_fields->refreshElapsed = 0.f;
        }
    }

    void updateOverlay() {
        if (!m_fields->overlay)
            return;

        auto director = CCDirector::sharedDirector();
        float interval = director ? director->getAnimationInterval() : 0.f;
        int fps = interval > 0.f ? static_cast<int>(0.5f + 1.f / interval) : 0;

        auto text = fmt::format(
            "FRAME WINDOW  |  +/- {} FRAMES\nTARGET FPS: {}",
            timingWindow(),
            fps > 0 ? std::to_string(fps) : std::string("N/A")
        );

        if (showDetails()) {
            text += fmt::format("\nCLICKS THIS ATTEMPT: {}", m_fields->clickCount);
            if (m_fields->lastButton >= 0) {
                text += fmt::format(
                    "\nLAST INPUT: BUTTON {} (PLAYER {})",
                    m_fields->lastButton,
                    m_fields->lastPlayer1 ? "1" : "2"
                );
            }
            text += fmt::format("\nOVERLAY UPDATES: {}", m_fields->overlayFrames);
            text += fmt::format("\nPROGRESS: {}%", this->getCurrentPercent());
        }
        m_fields->overlay->setString(text.c_str());
    }
};
