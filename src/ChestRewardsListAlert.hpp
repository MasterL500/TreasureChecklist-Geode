#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ChestRewardsListAlert : public Popup<> {
    protected:
        size_t m_currentPage = 1;
        size_t m_totalPages = 7;

        std::array<int, 7> m_chestTotal = {400, 100, 60, 24, 12, 8, 20};
        std::array<int, 7> m_chestCount = {0, 0, 0, 0, 0, 0, 0};

        bool setup() override;

        void createItem(CCMenu *, int, std::vector<int>);
        void createItemGroup(CCMenu *, std::map<int, int>);

        void createNavButton(CCMenu *, int, bool);
        void createIconPage(int, int);

        void onPageButton(CCObject *);
        void onNavButton(CCObject *);

        void loadData();

    public:
        static ChestRewardsListAlert * create();
        void onIcon(CCObject *);

};