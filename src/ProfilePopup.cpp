#include <Geode/Geode.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include "ProfilePopup.hpp"
#include <cue/ListBorder.hpp>
#include <cue/ListNode.hpp>
#include <cue/PlayerIcon.hpp>
#include "Geode/cocos/cocoa/CCGeometry.h"
#include "Geode/cocos/layers_scenes_transitions_nodes/CCLayer.h"
#include "Geode/ui/Layout.hpp"
#include "Geode/ui/NineSlice.hpp"

using namespace geode::prelude;

ProfilePopup* ProfilePopup::create(int accountId, bool ownProfile) {
    auto ret = new ProfilePopup();
    if (ret && ret->init(accountId, ownProfile)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ProfilePopup::init(int accountId, bool ownProfile) {
    if (!Popup::init(440.f, 290.f)) {
        return false;
    }

    // i hate these stuff
    m_noElasticity = true;
    if (m_buttonMenu) m_buttonMenu->removeMeAndCleanup();

    m_score = nullptr;

    // get the user's score info
    auto glm = GameLevelManager::get();
    glm->m_userInfoDelegate = this;
    glm->getGJUserInfo(accountId);

    m_score = glm->userInfoForAccountID(accountId);

    // left side panel
    m_closeMenu = CCMenu::create();
    m_closeMenu->setContentSize({35, 35});
    m_closeMenu->setID("close-menu");
    m_closeMenu->m_bIgnoreAnchorPointForPosition = false;
    m_closeMenu->setLayout(ColumnLayout::create());
    m_closeMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_closeMenu, Anchor::TopLeft, {30.f, -30.f}, {0.5, 0.5}, false);

    auto closeMenuBg = NineSlice::create("square02_small.png");
    closeMenuBg->setContentSize(m_closeMenu->getContentSize() + CCSize{5, 5});
    closeMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(closeMenuBg, Anchor::TopLeft, {30.f, -30.f}, {0.5, 0.5}, false);

    m_closeMenu->addChild(m_closeBtn);
    m_closeMenu->updateLayout();

    m_userOptionsMenu = CCMenu::create();
    m_userOptionsMenu->setContentSize({35, 135});
    m_userOptionsMenu->setID("user-options-menu");
    m_userOptionsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_userOptionsMenu->setLayout(ColumnLayout::create());
    m_userOptionsMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_userOptionsMenu, Anchor::Left, {30.f, 20.f}, {0.5, 0.5}, false);

    auto userOptionsMenuBg = NineSlice::create("square02_small.png");
    userOptionsMenuBg->setContentSize(m_userOptionsMenu->getContentSize() + CCSize{5, 5});
    userOptionsMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(userOptionsMenuBg, Anchor::Left, {30.f, 20.f}, {0.5, 0.5}, false);

    m_otherOptionsMenu = CCMenu::create();
    m_otherOptionsMenu->setContentSize({35, 70});
    m_otherOptionsMenu->setID("other-options-menu");
    m_otherOptionsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_otherOptionsMenu->setLayout(ColumnLayout::create());
    m_otherOptionsMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_otherOptionsMenu, Anchor::BottomLeft, {30.f, 50.f}, {0.5, 0.5}, false);

    auto otherOptionsMenuBg = NineSlice::create("square02_small.png");
    otherOptionsMenuBg->setContentSize(m_otherOptionsMenu->getContentSize() + CCSize{5, 5});
    otherOptionsMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(otherOptionsMenuBg, Anchor::BottomLeft, {30.f, 50.f}, {0.5, 0.5}, false);

    // center panel
    m_usernameMenu = CCMenu::create();
    m_usernameMenu->setContentSize({270, 25});
    m_usernameMenu->setID("username-menu");
    m_usernameMenu->m_bIgnoreAnchorPointForPosition = false;
    m_usernameMenu->setZOrder(1);
    m_usernameMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setCrossAxisOverflow(false)->setAutoScale(false));
    m_mainLayer->addChildAtPosition(m_usernameMenu, Anchor::Top, {-25.f, -25.f}, {0.5, 0.5}, false);

    m_statsMenu = CCMenu::create();
    m_statsMenu->setContentSize({270, 15});
    m_statsMenu->setID("stats-menu");
    m_statsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_statsMenu->setZOrder(1);
    m_statsMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setGap(2.5f)->setCrossAxisOverflow(false)->setAutoScale(false));
    m_mainLayer->addChildAtPosition(m_statsMenu, Anchor::Top, {-25.f, -50.f}, {0.5, 0.5}, false);

    m_iconsMenu = CCMenu::create();
    m_iconsMenu->setContentSize({315, 45});
    m_iconsMenu->setID("icons-menu");
    m_iconsMenu->setZOrder(1);
    m_iconsMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Center)->setGap(10.f)->setCrossAxisOverflow(false)->setAutoScale(true));
    m_mainLayer->addChildAtPosition(m_iconsMenu, Anchor::Center, {0.f, 55.f}, {0.5, 0.5}, false);

    auto iconsMenuBorder = cue::ListBorder::create(cue::ListBorderStyle::Comments, {325, 45}, ccColor4B{191, 114, 62, 255});
    iconsMenuBorder->m_bIgnoreAnchorPointForPosition = false;
    iconsMenuBorder->setZOrder(2);
    m_mainLayer->addChildAtPosition(iconsMenuBorder, Anchor::Center, {0.f, 55.f}, {0.5, 0.5}, false);

    auto iconsMenuBg = CCLayerColor::create({191, 114, 62, 255}, iconsMenuBorder->getContentSize().width, iconsMenuBorder->getContentSize().height);
    iconsMenuBg->m_bIgnoreAnchorPointForPosition = false;
    iconsMenuBg->setZOrder(0);
    m_mainLayer->addChildAtPosition(iconsMenuBg, Anchor::Center, {0.f, 55.f}, {0.5, 0.5}, false);

    m_commentsList = cue::ListNode::create({325, 70}, {191, 114, 62, 255}, cue::ListBorderStyle::Comments);
    m_commentsList->setID("comments-list");
    m_commentsList->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_commentsList, Anchor::Center, {0.f, -20.f}, {0.5, 0.5}, false);

    auto commentsListBg = CCLayerColor::create({191, 114, 62, 255}, m_commentsList->getContentSize().width, m_commentsList->getContentSize().height);
    commentsListBg->m_bIgnoreAnchorPointForPosition = false;
    commentsListBg->setZOrder(-1);
    m_commentsList->addChildAtPosition(commentsListBg, Anchor::Center, {0.f, 0.f}, {0.5, 0.5}, false);

    // right side panel
    m_refrshMenu = CCMenu::create();
    m_refrshMenu->setContentSize({35, 35});
    m_refrshMenu->setID("refresh-menu");
    m_refrshMenu->m_bIgnoreAnchorPointForPosition = false;
    m_refrshMenu->setLayout(ColumnLayout::create());
    m_refrshMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_refrshMenu, Anchor::TopRight, {-30.f, -30.f}, {0.5, 0.5}, false);

    auto refrshMenuBg = NineSlice::create("square02_small.png");
    refrshMenuBg->setContentSize(m_refrshMenu->getContentSize() + CCSize{5, 5});
    refrshMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(refrshMenuBg, Anchor::TopRight, {-30.f, -30.f}, {0.5, 0.5}, false);

    m_socialsMenu = CCMenu::create();
    m_socialsMenu->setContentSize({35, 135});
    m_socialsMenu->setID("socials-menu");
    m_socialsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_socialsMenu->setLayout(ColumnLayout::create());
    m_socialsMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_socialsMenu, Anchor::Right, {-30.f, 20.f}, {0.5, 0.5}, false);

    auto socialsMenuBg = NineSlice::create("square02_small.png");
    socialsMenuBg->setContentSize(m_socialsMenu->getContentSize() + CCSize{5, 5});
    socialsMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(socialsMenuBg, Anchor::Right, {-30.f, 20.f}, {0.5, 0.5}, false);

    m_onlineMenu = CCMenu::create();
    m_onlineMenu->setContentSize({35, 70});
    m_onlineMenu->setID("online-menu");
    m_onlineMenu->m_bIgnoreAnchorPointForPosition = false;
    m_onlineMenu->setLayout(ColumnLayout::create());
    m_onlineMenu->setZOrder(1);
    m_mainLayer->addChildAtPosition(m_onlineMenu, Anchor::BottomRight, {-30.f, 50.f}, {0.5, 0.5}, false);

    auto onlineMenuBg = NineSlice::create("square02_small.png");
    onlineMenuBg->setContentSize(m_onlineMenu->getContentSize() + CCSize{5, 5});
    onlineMenuBg->setOpacity(100);
    m_mainLayer->addChildAtPosition(onlineMenuBg, Anchor::BottomRight, {-30.f, 50.f}, {0.5, 0.5}, false);

    return true;
}

void ProfilePopup::getUserInfoFinished(GJUserScore* score) {
    m_score = score;
    if (m_score) {
        log::info("async user score received: username={}, stars={}, demons={}, creator points={}", m_score->m_userName, m_score->m_stars, m_score->m_demons, m_score->m_creatorPoints);
        // TODO: refresh UI elements that depend on m_score here
        auto usernameLabel = CCLabelBMFont::create(m_score->m_userName.c_str(), "bigFont.fnt");
        usernameLabel->setScale(0.8f);
        m_usernameMenu->addChild(usernameLabel);

        m_usernameMenu->updateLayout();

        // stats
        auto starsLabel = CCLabelBMFont::create(GameToolbox::pointsToString(m_score->m_stars).c_str(), "bigFont.fnt");
        starsLabel->setScale(0.4f);
        starsLabel->setColor({233, 253, 113});
        m_statsMenu->addChild(starsLabel);

        auto starsIcon = CCSprite::createWithSpriteFrameName("GJ_starsIcon_001.png");
        starsIcon->setScale(0.5f);
        m_statsMenu->addChild(starsIcon);

        // moons
        auto moonLabel = CCLabelBMFont::create(GameToolbox::pointsToString(m_score->m_moons).c_str(), "bigFont.fnt");
        moonLabel->setScale(0.4f);
        moonLabel->setColor({109, 215, 249});
        m_statsMenu->addChild(moonLabel);

        auto moonIcon = CCSprite::createWithSpriteFrameName("GJ_moonsIcon_001.png");
        moonIcon->setScale(0.5f);
        m_statsMenu->addChild(moonIcon);

        // demons
        auto demonLabel = CCLabelBMFont::create(GameToolbox::pointsToString(m_score->m_demons).c_str(), "bigFont.fnt");
        demonLabel->setScale(0.4f);
        demonLabel->setColor({240, 140, 140});
        m_statsMenu->addChild(demonLabel);

        auto demonIcon = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
        demonIcon->setScale(0.5f);
        m_statsMenu->addChild(demonIcon);

        // creator points
        if (m_score->m_creatorPoints > 0) {
            auto cpLabel = CCLabelBMFont::create(GameToolbox::pointsToString(m_score->m_creatorPoints).c_str(), "bigFont.fnt");
            cpLabel->setScale(0.4f);
            cpLabel->setColor({182, 186, 186});
            m_statsMenu->addChild(cpLabel);

            auto cpIcon = CCSprite::createWithSpriteFrameName("GJ_hammerIcon_001.png");
            cpIcon->setScale(0.5f);
            m_statsMenu->addChild(cpIcon);
        }

        // user coints
        auto userCoinsLabel = CCLabelBMFont::create(GameToolbox::pointsToString(m_score->m_userCoins).c_str(), "bigFont.fnt");
        userCoinsLabel->setScale(0.4f);
        userCoinsLabel->setColor({255, 255, 255});
        m_statsMenu->addChild(userCoinsLabel);

        auto userCoinsIcon = CCSprite::createWithSpriteFrameName("GJ_coinsIcon2_001.png");
        userCoinsIcon->setScale(0.5f);
        m_statsMenu->addChild(userCoinsIcon);

        m_statsMenu->updateLayout();

        // icons
        auto playerObject = [&](IconType iconType, int playerType) {
            auto wrapper = CCNode::create();
            wrapper->setContentSize({35, 35});
            wrapper->setAnchorPoint({0.5f, 0.5f});

            auto player = cue::PlayerIcon::create(iconType, playerType, m_score->m_color1, m_score->m_color2, m_score->m_color3);
            player->setAnchorPoint({0.5f, 0.5f});
            player->setPosition({wrapper->getContentSize().width / 2.f, wrapper->getContentSize().height / 2.f});
            wrapper->addChild(player);

            if (iconType == IconType::Ufo) player->setPositionY(player->getPositionY() - 7);  // very hacky way to make the ufo aligned
            return wrapper;
        };

        m_iconsMenu->addChild(playerObject(IconType::Cube, m_score->m_playerCube));
        m_iconsMenu->addChild(playerObject(IconType::Ship, m_score->m_playerShip));
        m_iconsMenu->addChild(playerObject(IconType::Ball, m_score->m_playerBall));
        m_iconsMenu->addChild(playerObject(IconType::Ufo, m_score->m_playerUfo));
        m_iconsMenu->addChild(playerObject(IconType::Wave, m_score->m_playerWave));
        m_iconsMenu->addChild(playerObject(IconType::Robot, m_score->m_playerRobot));
        m_iconsMenu->addChild(playerObject(IconType::Spider, m_score->m_playerSpider));
        m_iconsMenu->addChild(playerObject(IconType::Swing, m_score->m_playerSwing));
        m_iconsMenu->addChild(playerObject(IconType::Jetpack, m_score->m_playerJetpack));

        m_iconsMenu->updateLayout();
    }
}

void ProfilePopup::getUserInfoFailed(int id) {
    log::error("user info request failed for account id {}", id);
}

void ProfilePopup::userInfoChanged(GJUserScore* score) {
    if (score && m_score && score == m_score) {
        m_score = score;
        log::info("user score changed for account id {}", m_score->m_accountID);
        // TODO: refresh UI elements if needed
    }
}
