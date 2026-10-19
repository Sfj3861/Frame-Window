#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr char const* kOverlayID = "frame-window-practice-overlay";
    bool overlayEnabled() { return Mod::get()->getSettingValue<bool>("show-overlay"); }
    int timingWindow() { return Mod::get()->getSettingValue<int>("window-frames"); }
    bool showDetails() { return Mod::get()->getSettingValue<bool>("show-details"); }
}

class $modify(FrameWindowMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Frame Window", 90, true, "bigFont.fnt", "GJ_button_01.png", 30.0f, 0.8f),
            this, menu_selector(FrameWindowMenuLayer::onFrameWindow)
        );
        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({winSize.width - 65.0f, 45.0f});
        this->addChild(menu, 10);
        return true;
    }

    void onFrameWindow(CCObject*) {
        auto message = fmt::format(
            "PRACTICE CONFIGURATION\n\nTiming window: +/- {} frames\nOverlay: {}\nExtra stats: {}\n\n"
            "Real button DOWN/UP events are recorded in the Geode log with time and frame number. "
            "This mod does not alter inputs, collisions, hitboxes, or physics.",
            timingWindow(), overlayEnabled() ? "ON" : "OFF", showDetails() ? "ON" : "OFF"
        );
        FLAlertLayer::create("Frame Window v0.2.2", message, "OK")->show();
    }
};

class $modify(FrameWindowPlayLayer, PlayLayer) {
    struct Fields {
        CCLabelBMFont* overlay = nullptr;
        float elapsed = 0.f;
        float refreshElapsed = 0.f;
        unsigned long long frameCount = 0;
        unsigned long long overlayFrames = 0;
        unsigned long long pressCount = 0;
        unsigned long long releaseCount = 0;
        int lastButton = -1;
        bool lastPlayer1 = true;
        bool lastDown = false;
    };

    void onEnter() {
        PlayLayer::onEnter();
        log::info("[Frame Window] Attempt started; input recorder enabled.");
        if (!overlayEnabled()) return;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto label = CCLabelBMFont::create("", "bigFont.fnt");
        if (!label) return;
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
        if (down) ++m_fields->pressCount;
        else ++m_fields->releaseCount;

        m_fields->lastButton = button;
        m_fields->lastPlayer1 = isPlayer1;
        m_fields->lastDown = down;

        log::info("[Frame Window] t={:.4f}s frame={} event={} button={} player={}",
            m_fields->elapsed, m_fields->frameCount, down ? "DOWN" : "UP",
            button, isPlayer1 ? 1 : 2);

        PlayLayer::handleButton(down, button, isPlayer1);
    }

    void update(float dt) {
        PlayLayer::update(dt);
        m_fields->elapsed += dt;
        ++m_fields->frameCount;

        if (!m_fields->overlay || !overlayEnabled()) return;
        m_fields->refreshElapsed += dt;
        ++m_fields->overlayFrames;
        if (m_fields->refreshElapsed >= 0.1f) {
            updateOverlay();
            m_fields->refreshElapsed = 0.f;
        }
    }

    void updateOverlay() {
        if (!m_fields->overlay) return;
        auto director = CCDirector::sharedDirector();
        float interval = director ? director->getAnimationInterval() : 0.f;
        int fps = interval > 0.f ? static_cast<int>(0.5f + 1.f / interval) : 0;

        auto text = fmt::format("FRAME WINDOW | +/- {} FRAMES\nTARGET FPS: {}",
            timingWindow(), fps > 0 ? std::to_string(fps) : std::string("N/A"));

        if (showDetails()) {
            text += fmt::format("\nINPUTS DOWN: {}", m_fields->pressCount);
            text += fmt::format("\nINPUTS UP: {}", m_fields->releaseCount);
            if (m_fields->lastButton >= 0) {
                text += fmt::format("\nLAST INPUT: {} BUTTON {} (PLAYER {})",
                    m_fields->lastDown ? "DOWN" : "UP", m_fields->lastButton,
                    m_fields->lastPlayer1 ? "1" : "2");
            }
            text += fmt::format("\nFRAME COUNT: {}", m_fields->frameCount);
            text += fmt::format("\nOVERLAY UPDATES: {}", m_fields->overlayFrames);
            text += fmt::format("\nPROGRESS: {}%", this->getCurrentPercent());
        }
        m_fields->overlay->setString(text.c_str());
    }
};
