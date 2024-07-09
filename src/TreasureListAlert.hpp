#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class TreasureListAlert : public Popup<> {
    protected:
        size_t m_currentPage = 1;
        std::array<int, 6> m_chestTotal = {400, 100, 60, 24, 12, 8};
        std::array<int, 6> m_chestCount = {0, 0, 0, 0, 0, 0};

        bool setup() override;

        void createItem(CCMenu *, int, std::vector<int>);
        void createItemGroup(CCMenu *, std::map<int, int>);

        void createNavButton(CCMenu *, int, bool);
        void createIconPage(int, int);

        void onNavButton(CCObject *);
        void onPageButton(CCObject *);

        void loadData();

    public:
        static TreasureListAlert * create();
        void onIcon(CCObject *);

};