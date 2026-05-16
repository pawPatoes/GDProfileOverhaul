#include <Geode/Geode.hpp>
#include <cue/ListBorder.hpp>
#include <cue/ListNode.hpp>

using namespace geode::prelude;

class ProfilePopup : public geode::Popup, public UserInfoDelegate {
    friend ProfilePage;

public:
    static ProfilePopup* create(int accountId, bool ownProfile);
    bool init(int accountId, bool ownProfile);

    void getUserInfoFinished(GJUserScore* score) override;
    void getUserInfoFailed(int id) override;
    void userInfoChanged(GJUserScore* score) override;

private:
    GJUserScore* m_score;

    // left side panel
    CCMenu* m_closeMenu;
    CCMenu* m_userOptionsMenu;
    CCMenu* m_otherOptionsMenu;

    // center panel
    CCMenu* m_usernameMenu;
    CCMenu* m_statsMenu;
    CCMenu* m_iconsMenu;
    cue::ListNode* m_commentsList;
    cue::ListBorder* m_ratedLevelCell;

    // right side panel
    CCMenu* m_refrshMenu;
    CCMenu* m_socialsMenu;
    CCMenu* m_onlineMenu;
};