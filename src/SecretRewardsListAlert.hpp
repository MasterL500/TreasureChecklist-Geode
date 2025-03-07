#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SecretRewardsListAlert : public Popup<> {
    public:
        unsigned int m_page = 1;
    protected:
        unsigned int m_totalPages = 8;

        std::array<int, 8> m_chestCount = {0, 0, 0, 0, 0, 0, 0, 0};
        std::array<int, 8> m_chestTotal = {0, 0, 0, 0, 0, 0, 0, 0};

        bool setup() override;

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
        static SecretRewardsListAlert * create();
        void onIcon(CCObject *);
        void onNavButton(CCObject *);

};