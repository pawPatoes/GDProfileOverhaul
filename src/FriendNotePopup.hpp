#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/GJUserScore.hpp>
#include "Geode/ui/Popup.hpp"
#include "ProfilePopup.hpp"

using namespace geode::prelude;

std::pair<std::string, std::string> loadFriendNoteFromFile(int accountId);

class FriendNotePopup : public geode::Popup {
public:
    static FriendNotePopup* create(ProfilePopup* profilePopup, GJUserScore* score);

private:
    bool init(ProfilePopup* profilePopup, GJUserScore* score);
    TextInput* m_friendNoteInput;
    TextInput* m_nickNameInput;
};