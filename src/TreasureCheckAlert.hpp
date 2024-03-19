#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class TreasureCheckAlert : public Popup<> {
    protected:
        std::array<int, 6> m_chestTotal = {400, 100, 60, 24, 12, 8};
        std::array<int, 6> m_chestCount = {0, 0, 0, 0, 0, 0};

        std::array<int, 6> m_iconsTotal = {0, 0, 0, 0, 0, 0};
        std::array<int, 6> m_iconsCount = {0, 0, 0, 0, 0, 0};

        bool setup() override;
        void loadTreasureRoomInfo();

        CCMenu* createIconPage(int, bool);
        void createChestButton(CCMenu*, int, bool);
        
        template <typename T>
        void addIcons(CCMenu*, int, const T &);
        template <typename T, typename... A>
        void addIcons(CCMenu*, int, const T &, const A &...);
        void addGroupedIcons(CCMenu*, int, int, int, int, int = 0, int = 0, int = 0, int = 0);

        void onChestButton(CCObject*);
        void onIconButton(CCObject*);
        void onInfoButton(CCObject*);

    public:
        static TreasureCheckAlert * create();
};