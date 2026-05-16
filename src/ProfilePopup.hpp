#include <Geode/Geode.hpp>
#include <cue/ListBorder.hpp>
#include <cue/ListNode.hpp>

using namespace geode::prelude;

class ProfilePopup : public geode::Popup, public UserInfoDelegate, public LevelCommentDelegate, public LeaderboardManagerDelegate {
public:
    static ProfilePopup* create(int accountId, bool ownProfile);
    bool init(int accountId, bool ownProfile);

private:
    // left side panel
    CCMenu* m_closeMenu;
    CCMenu* m_userOptionsMenu;
    CCMenu* m_otherOptionsMenu;

    // center panel
    CCMenu* m_usernameMenu;
    CCMenu* m_statsMenu;
    cue::ListBorder* m_iconsMenu;
    cue::ListNode* m_commentsList;
    cue::ListBorder* m_ratedLevelCell;

    // right side panel
    CCMenu* m_refrshMenu;
    CCMenu* m_socialsMenu;
    CCMenu* m_onlineMenu;

protected:
    void onClose(CCObject* sender) override;
};