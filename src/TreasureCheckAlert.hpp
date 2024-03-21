#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

enum class rewardType {
    none    = 0x0,
    cube    = 0x1,
    col1    = 0x2,
    col2    = 0x3,
    ship    = 0x4,
    ball    = 0x5,
    bird    = 0x6,
    dart    = 0x7,
    robot   = 0x8,
    spider  = 0x9,
    effect  = 0xB,
    swing   = 0xD,
};

class TreasureCheckAlert : public Popup<> {
    protected:
        std::array<int, 6> m_chestTotal = {400, 100, 60, 24, 12, 8};
        std::array<int, 6> m_chestCount = {0, 0, 0, 0, 0, 0};

        bool setup() override;
        void loadInfo();

        CCMenu * createIconMenu(CCSize limits, int ID, bool isVisible);
        void createPageButton(CCMenu * menu, int tag, bool isActive);

        template <typename T>
        void addIcons(CCMenu * menu, rewardType type, const T & iconID);

        template <typename T, typename... A>
        void addIcons(CCMenu * menu, rewardType type, const T & iconID, const A &... args);

        void addIconGroup(CCMenu * menu, rewardType firstType, int firstID, rewardType secondType, int secondID, rewardType thirdType = rewardType::none, int thirdID = 0, rewardType fourthType = rewardType::none, int fourthID = 0);

        void onPageButton(CCObject *);
        void onInfoButton(CCObject *);
        void onIconClick(CCObject *);

    public:
        static TreasureCheckAlert * create();
};