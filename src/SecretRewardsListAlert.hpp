#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SecretRewardsListAlert : public Popup<> {
    protected:
        unsigned int m_page = 1;
        unsigned int m_totalPages = 8;

        std::array<int, 8> m_chestCount = {0, 0, 0, 0, 0, 0, 0, 0};
        std::array<int, 8> m_chestTotal = {51, 33, 60, 48, 36, 32, 20, 30};

        bool setup() override;

        void createItem(CCMenu *, int, std::vector<int>);
        void createItemGroup(CCMenu *, std::map<int, int>);
        void createItemLabeled(CCMenu *, std::pair<int, int>, const char *);

        void createNavButton(CCMenu *, int, bool);
        void createIconPage(int, int);

        void onPageButton(CCObject *);
        void onNavButton(CCObject *);
        void onSettings(CCObject *);
        void onInfo(CCObject *);

        void loadData();

    public:
        static SecretRewardsListAlert * create();
        void onIcon(CCObject *);

};