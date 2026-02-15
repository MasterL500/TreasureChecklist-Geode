#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SecretRewardsListAlert : public Popup
{
protected:
    std::array<int, 8> m_chestCount = {0, 0, 0, 0, 0, 0, 0, 0};
    std::array<int, 8> m_chestTotal = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int m_totalPages = 8;

    bool init() override;

    void createItem(CCMenu *, UnlockType, int);
    void createItemGroup(CCMenu *, std::vector<std::pair<UnlockType, int>>);
    void createItemLabeled(CCMenu *, std::pair<UnlockType, int>, const char *);
    void createNavButton(CCMenu *, int, bool);
    void createIconPage(int, int);

    void onPageButton(CCObject *);
    void onSettings(CCObject *);
    void onInfo(CCObject *);

    void loadData();

public:
    unsigned int m_page = 1;

    static SecretRewardsListAlert *create();
    void onNavButton(CCObject *);
    void onIcon(CCObject *);
};