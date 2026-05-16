#include <Geode/Geode.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include "ProfilePopup.hpp"
#include <cue/ListNode.hpp>
#include "Geode/cocos/cocoa/CCGeometry.h"
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

    // get the user's score info
    GJUserScore* m_score = GameLevelManager::get()->userInfoForAccountID(accountId);

    if (m_score) {
        log::info("got user score for account id {}: username={}, stars={}, demons={}, creator points={}", accountId, m_score->m_userName, m_score->m_stars, m_score->m_demons, m_score->m_creatorPoints);
    } else {
        log::error("failed to get user score for account id {}", accountId);
    }

    // left side panel
    m_closeMenu = CCMenu::create();
    m_closeMenu->setContentSize({35, 35});
    m_closeMenu->setID("close-menu");
    m_closeMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_closeMenu, Anchor::TopLeft, {30.f, -30.f}, {0.5, 0.5}, false);

    auto closeMenuBg = NineSlice::create("square02_small.png");
    closeMenuBg->setContentSize(m_closeMenu->getContentSize() + CCSize{5, 5});
    closeMenuBg->setZOrder(-1);
    closeMenuBg->setOpacity(100);
    m_closeMenu->addChildAtPosition(closeMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    m_userOptionsMenu = CCMenu::create();
    m_userOptionsMenu->setContentSize({35, 135});
    m_userOptionsMenu->setID("user-options-menu");
    m_userOptionsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_userOptionsMenu, Anchor::Left, {30.f, 20.f}, {0.5, 0.5}, false);

    auto userOptionsMenuBg = NineSlice::create("square02_small.png");
    userOptionsMenuBg->setContentSize(m_userOptionsMenu->getContentSize() + CCSize{5, 5});
    userOptionsMenuBg->setZOrder(-1);
    userOptionsMenuBg->setOpacity(100);
    m_userOptionsMenu->addChildAtPosition(userOptionsMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    m_otherOptionsMenu = CCMenu::create();
    m_otherOptionsMenu->setContentSize({35, 70});
    m_otherOptionsMenu->setID("other-options-menu");
    m_otherOptionsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_otherOptionsMenu, Anchor::BottomLeft, {30.f, 50.f}, {0.5, 0.5}, false);

    auto otherOptionsMenuBg = NineSlice::create("square02_small.png");
    otherOptionsMenuBg->setContentSize(m_otherOptionsMenu->getContentSize() + CCSize{5, 5});
    otherOptionsMenuBg->setZOrder(-1);
    otherOptionsMenuBg->setOpacity(100);
    m_otherOptionsMenu->addChildAtPosition(otherOptionsMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    // center panel
    m_usernameMenu = CCMenu::create();
    m_usernameMenu->setContentSize({270, 25});
    m_usernameMenu->setID("username-menu");
    m_usernameMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_usernameMenu, Anchor::Top, {-30.f, -25.f}, {0.5, 0.5}, false);

    m_statsMenu = CCMenu::create();
    m_statsMenu->setContentSize({270, 20});
    m_statsMenu->setID("stats-menu");
    m_statsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_statsMenu, Anchor::Top, {-30.f, -45.f}, {0.5, 0.5}, false);

    // right side panel
    m_refrshMenu = CCMenu::create();
    m_refrshMenu->setContentSize({35, 35});
    m_refrshMenu->setID("refresh-menu");
    m_refrshMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_refrshMenu, Anchor::TopRight, {-30.f, -30.f}, {0.5, 0.5}, false);

    auto refrshMenuBg = NineSlice::create("square02_small.png");
    refrshMenuBg->setContentSize(m_refrshMenu->getContentSize() + CCSize{5, 5});
    refrshMenuBg->setZOrder(-1);
    refrshMenuBg->setOpacity(100);
    m_refrshMenu->addChildAtPosition(refrshMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    m_socialsMenu = CCMenu::create();
    m_socialsMenu->setContentSize({35, 135});
    m_socialsMenu->setID("socials-menu");
    m_socialsMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_socialsMenu, Anchor::Right, {-30.f, 20.f}, {0.5, 0.5}, false);

    auto socialsMenuBg = NineSlice::create("square02_small.png");
    socialsMenuBg->setContentSize(m_socialsMenu->getContentSize() + CCSize{5, 5});
    socialsMenuBg->setZOrder(-1);
    socialsMenuBg->setOpacity(100);
    m_socialsMenu->addChildAtPosition(socialsMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    m_onlineMenu = CCMenu::create();
    m_onlineMenu->setContentSize({35, 70});
    m_onlineMenu->setID("online-menu");
    m_onlineMenu->m_bIgnoreAnchorPointForPosition = false;
    m_mainLayer->addChildAtPosition(m_onlineMenu, Anchor::BottomRight, {-30.f, 50.f}, {0.5, 0.5}, false);

    auto onlineMenuBg = NineSlice::create("square02_small.png");
    onlineMenuBg->setContentSize(m_onlineMenu->getContentSize() + CCSize{5, 5});
    onlineMenuBg->setZOrder(-1);
    onlineMenuBg->setOpacity(100);
    m_onlineMenu->addChildAtPosition(onlineMenuBg, Anchor::Center, {}, {0.5, 0.5}, false);

    return true;
};
