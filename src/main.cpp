#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(FrameWindowMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto button = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Frame Window", 90, true, "bigFont.fnt", "GJ_button_01.png", 30.0f, 0.8f),
            this,
            menu_selector(FrameWindowMenuLayer::onFrameWindow)
        );

        auto menu = CCMenu::create();
        menu->addChild(button);
        menu->setPosition({CCDirector::sharedDirector()->getWinSize().width - 65.0f, 45.0f});
        this->addChild(menu, 10);
        return true;
    }

    void onFrameWindow(CCObject*) {
        FLAlertLayer::create(
            "Frame Window",
            "Input recorder is enabled during gameplay.\n\n"
            "This mod records button DOWN/UP events and frame timestamps. "
            "It does not automate inputs or modify physics.",
            "OK"
        )->show();
    }
};

class $modify(FrameWindowPlayLayer, PlayLayer) {
    struct Fields {
        unsigned long long frameCount = 0;
        unsigned long long pressCount = 0;
        unsigned long long releaseCount = 0;
        float elapsed = 0.f;
    };

    void onEnter() {
        PlayLayer::onEnter();
        m_fields->frameCount = 0;
        m_fields->pressCount = 0;
        m_fields->releaseCount = 0;
        m_fields->elapsed = 0.f;
        log::info("[Frame Window] Input recorder started.");
    }

    void update(float dt) {
        PlayLayer::update(dt);
        ++m_fields->frameCount;
        m_fields->elapsed += dt;
    }

    void handleButton(bool down, int button, bool isPlayer1) {
        if (down) {
            ++m_fields->pressCount;
        } else {
            ++m_fields->releaseCount;
        }

        log::info(
            "[Frame Window] t={:.4f}s frame={} event={} button={} player={} downCount={} upCount={}",
            m_fields->elapsed,
            m_fields->frameCount,
            down ? "DOWN" : "UP",
            button,
            isPlayer1 ? 1 : 2,
            m_fields->pressCount,
            m_fields->releaseCount
        );

        PlayLayer::handleButton(down, button, isPlayer1);
    }
};
