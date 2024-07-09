#include "TreasureListAlert.hpp"

//  Structure for Icons
struct IconParameters : public CCObject{
    int p_iconType;
    int p_iconID;
    int p_chestID;

    /// @brief
    /// @param icon
    /// @param id

    IconParameters(int type, int id, int chest) : p_iconType(type), p_iconID(id), p_chestID(chest) {
        this->autorelease();
    }
};

bool TreasureListAlert::setup(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    
    //  Background for the Icon Lists
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({winSize.width / 2.f, winSize.height / 2.f + 2.f});
    iconListBG->setColor({ 133, 68, 41 });
    iconListBG->setContentSize({430, 176});
	this->addChild(iconListBG);

    //  Info Button (Menu)
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({405.f, 215.f});
    infoMenu->ignoreAnchorPointForPosition(false);
    infoMenu->setID("info-menu");

    //  Info Button (Itself)
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        nullptr //menu_selector(TreasureListAlert::onInfoButton)
    );

    infoButton->setPosition(infoMenu->getContentSize());
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);
    //  this->addChild(infoMenu);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({winSize.width / 2, winSize.height / 2 - 104.0f});
    navMenu->setLayout(RowLayout::create()
            ->setGap(6.f));
    navMenu->setID("navigation-menu");
    this->addChild(navMenu);

    //  Navigation Menu (Buttons)
    for (int ii = 1; ii <= 6; ii++){
        createNavButton(navMenu, ii, ii == 1);
    };

    /*
    //  Arrow Buttons for shops with more than One page (FOR THE FUTURE)
    auto pageNavMenu = CCMenu::create();
    pageNavMenu->setLayout(RowLayout::create()
                ->setGap(460.0f)
                ->setGrowCrossAxis(true)
                ->setCrossAxisOverflow(false));
    pageNavMenu->setID("page-menu");
    this->addChild(pageNavMenu);

    auto prevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        nullptr // menu_selector(TreasureListAlert::onPageButton)
    );

    prevBtn->setID("arrow-prev-button");
    prevBtn->setTag(1);

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);

    auto nextBtn = CCMenuItemSpriteExtra::create(
        nextSpr,
        this,
        nullptr // menu_selector(TreasureListAlert::onPageButton)
    );

    nextBtn->setID("arrow-next-button");
    nextBtn->setScale(-1);
    nextBtn->setTag(2);

    pageNavMenu->addChild(prevBtn);
    pageNavMenu->addChild(nextBtn);
    pageNavMenu->setVisible(false);
    pageNavMenu->updateLayout();
    */

    //  Creates the Menu
    auto iconMenu = CCMenu::create();
    iconMenu->setContentSize({530.0f, 220.0f});
    iconMenu->setLayout(RowLayout::create()
            ->setGap(14.0f)
            ->setAutoScale(false)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisLineAlignment(AxisAlignment::Even));

    iconMenu->setID("icon-menu");
    iconMenu->setPositionY(winSize.height / 2 + 2.f);
    iconMenu->setScale(0.8f);
    this->addChild(iconMenu);

    createIconPage(1, 1);

    this->m_noElasticity = true;
    this->setTitle("Rewards (1x Key Chests)");
    return true;
};

void TreasureListAlert::createIconPage(int ID, int index){
    auto menu = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);

    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto groupRewards = Mod::get()->getSettingValue<bool>("group-by-chest");

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);

    iconMenu->setContentSize({530.0f, 220.0f});
    iconMenu->setLayout(RowLayout::create()
            ->setGap(14.0f)
            ->setAutoScale(false)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisLineAlignment(AxisAlignment::Even));

    switch (ID){
        case 1:
            createItem(iconMenu, 0x1, {97, 163, 167, 176, 179, 189, 245, 249, 255, 278, 281, 282, 285, 287, 294, 302, 313, 333, 334, 335, 348, 350, 353, 373, 391, 414, 420, 426, 428});
            createItem(iconMenu, 0x4, {26, 69, 83});
            createItem(iconMenu, 0x5, {21});
            createItem(iconMenu, 0x6, {19, 118});
            createItem(iconMenu, 0x7, {16});
            createItem(iconMenu, 0x8, {10});
            createItem(iconMenu, 0x9, {7});

            if(showMiscRewards){
                createItem(iconMenu, 0x2, {41, 101});
                createItem(iconMenu, 0x3, {40, 58, 61, 63, 66, 67, 69, 76, 78, 80});
                createItem(iconMenu, 0xB, {3});
            }
        break;

        case 2:
            createItem(iconMenu, 0x1, {112, 114, 115, 116, 137, 155, 171, 175, 177, 180, 265, 269, 273, 289, 291, 322, 330});
            createItem(iconMenu, 0x4, {51, 73});
            createItem(iconMenu, 0x5, {39, 42, 58});
            createItem(iconMenu, 0x6, {53, 56, 97});
            createItem(iconMenu, 0x7, {34, 42, 76});
            createItem(iconMenu, 0x8, {26, 30});
            createItem(iconMenu, 0x9, {17});

            if(showMiscRewards){
                createItem(iconMenu, 0xB, {14, 15});
            }
        break;

        case 3:
            createItem(iconMenu, 0x1, {212, 218, 228, 256, 259, 298, 300, 318, 328, 331, 336, 338, 340, 345, 349, 355, 366, 374, 380, 382, 389, 392, 398, 402, 410, 418, 432, 437, 441, 461});
            createItem(iconMenu, 0x4, {64, 71, 89, 110});
            createItem(iconMenu, 0x5, {85, 87, 101, 109});
            createItem(iconMenu, 0x6, {81, 103, 123});
            createItem(iconMenu, 0x7, {54, 91});
            createItem(iconMenu, 0x8, {27, 61});
            createItem(iconMenu, 0x9, {34, 52, 63});
            createItem(iconMenu, 0xD, {14, 28, 29});

            if(showMiscRewards){
                createItem(iconMenu, 0x3, {64, 75, 81, 82, 83, 84, 101, 103, 105});
            }
        break;

        case 4:
            if(!groupRewards || !showMiscRewards){
                createItem(iconMenu, 0x1, {293, 295, 404, 405, 406, 419});
                createItem(iconMenu, 0x4, {59, 72, 112});
                createItem(iconMenu, 0x5, {88, 89});
                createItem(iconMenu, 0x6, {36, 58, 61, 67, 99, 126});
                createItem(iconMenu, 0x7, {47, 73});
                createItem(iconMenu, 0x8, {64, 67});
                createItem(iconMenu, 0xD, {11, 13, 43});

                if(showMiscRewards){
                    createItem(iconMenu, 0x2, {42, 43, 47, 50, 55, 66, 67, 68, 69, 70, 72, 73, 75, 78, 79, 81, 87, 88, 90});
                    createItem(iconMenu, 0x3, {42, 60, 74, 98});
                }
            } else {
                iconMenu->setLayout(RowLayout::create()
                    ->setGap(14.0f)
                    ->setGrowCrossAxis(true)
                    ->setCrossAxisOverflow(false)
                    ->setCrossAxisLineAlignment(AxisAlignment::Even)
                );

                createItemGroup(iconMenu, {{0x1, 293}, {0x3, 98}});
                createItemGroup(iconMenu, {{0x1, 295}, {0x3, 42}});
                createItemGroup(iconMenu, {{0x1, 404}, {0x2, 70}});
                createItemGroup(iconMenu, {{0x1, 405}, {0x2, 69}});
                createItemGroup(iconMenu, {{0x1, 406}, {0x2, 68}});
                createItemGroup(iconMenu, {{0x1, 419}, {0x2, 72}});
                createItemGroup(iconMenu, {{0x4, 59}, {0x2, 78}});
                createItemGroup(iconMenu, {{0x4, 72}, {0x2, 73}});
                createItemGroup(iconMenu, {{0x4, 112}, {0x2, 43}});
                createItemGroup(iconMenu, {{0x5, 88}, {0x2, 79}});
                createItemGroup(iconMenu, {{0x5, 89}, {0x2, 50}});
                createItemGroup(iconMenu, {{0x6, 39}, {0x2, 42}});
                createItemGroup(iconMenu, {{0x6, 58}, {0x2, 75}});
                createItemGroup(iconMenu, {{0x6, 61}, {0x2, 81}});
                createItemGroup(iconMenu, {{0x6, 67}, {0x2, 55}});
                createItemGroup(iconMenu, {{0x6, 99}, {0x3, 60}});
                createItemGroup(iconMenu, {{0x6, 126}, {0x2, 87}});
                createItemGroup(iconMenu, {{0x7, 47}, {0x2, 88}});
                createItemGroup(iconMenu, {{0x7, 73}, {0x3, 74}});
                createItemGroup(iconMenu, {{0x8, 64}, {0x2, 47}});
                createItemGroup(iconMenu, {{0x8, 67}, {0x2, 46}});
                createItemGroup(iconMenu, {{0xD, 11}, {0x2, 67}});
                createItemGroup(iconMenu, {{0xD, 13}, {0x2, 66}});
                createItemGroup(iconMenu, {{0xD, 43}, {0x2, 90}});
            }
        break;

        case 5:
            if(!groupRewards){
                createItem(iconMenu, 0x1, {166, 202, 261, 264, 280, 301, 361, 384, 393});
                createItem(iconMenu, 0x4, {82, 85, 97, 135});
                createItem(iconMenu, 0x5, {56, 115});
                createItem(iconMenu, 0x6, {87, 142, 143});
                createItem(iconMenu, 0x7, {79, 96});
                createItem(iconMenu, 0x8, {36, 50});
                createItem(iconMenu, 0x9, {47, 62});

                if(showMiscRewards){
                    createItem(iconMenu, 0x3, {44, 45, 46, 48, 50, 55, 87, 90, 92, 93, 95, 97});
                }
            } else {
                iconMenu->setLayout(RowLayout::create()
                    ->setGap(24.0f)
                    ->setGrowCrossAxis(true)
                    ->setCrossAxisOverflow(false)
                    ->setCrossAxisAlignment(AxisAlignment::Center)
                    ->setCrossAxisLineAlignment(AxisAlignment::Even)
                );
                iconMenu->setContentSize({440.f, 220.f});

                if (showMiscRewards){
                    createItemGroup(iconMenu, {{0x1, 384}, {0x4, 97}, {0x3, 87}});
                    createItemGroup(iconMenu, {{0x1, 280}, {0x6, 87}, {0x3, 90}});
                    createItemGroup(iconMenu, {{0x1, 166}, {0x4, 82}, {0x3, 92}});
                    createItemGroup(iconMenu, {{0x5, 56}, {0x9, 47}, {0x3, 93}});
                    createItemGroup(iconMenu, {{0x6, 142}, {0x1, 264}, {0x3, 95}});
                    createItemGroup(iconMenu, {{0x7, 96}, {0x4, 85}, {0x3, 97}});
                    createItemGroup(iconMenu, {{0x1, 301}, {0x4, 135}, {0x3, 44}});
                    createItemGroup(iconMenu, {{0x7, 79}, {0x1, 202}, {0x3, 45}});
                    createItemGroup(iconMenu, {{0x1, 261}, {0x6, 143}, {0x3, 46}});
                    createItemGroup(iconMenu, {{0x8, 36}, {0x5, 115}, {0x3, 48}});
                    createItemGroup(iconMenu, {{0x9, 62}, {0x1, 361}, {0x3, 50}});
                    createItemGroup(iconMenu, {{0x8, 50}, {0x1, 393}, {0x3, 55}});
                } else {
                    createItemGroup(iconMenu, {{0x1, 384}, {0x4, 97}});
                    createItemGroup(iconMenu, {{0x1, 280}, {0x6, 87}});
                    createItemGroup(iconMenu, {{0x1, 166}, {0x4, 82}});
                    createItemGroup(iconMenu, {{0x5, 56}, {0x9, 47}});
                    createItemGroup(iconMenu, {{0x6, 142}, {0x1, 264}});
                    createItemGroup(iconMenu, {{0x7, 96}, {0x4, 85}});
                    createItemGroup(iconMenu, {{0x1, 301}, {0x4, 135}});
                    createItemGroup(iconMenu, {{0x7, 79}, {0x1, 202}});
                    createItemGroup(iconMenu, {{0x1, 261}, {0x6, 143}});
                    createItemGroup(iconMenu, {{0x8, 36}, {0x5, 115}});
                    createItemGroup(iconMenu, {{0x9, 62}, {0x1, 361}});
                    createItemGroup(iconMenu, {{0x8, 50}, {0x1, 393}});
                }
                
            }
        break;

        case 6:
            if(!groupRewards){
                createItem(iconMenu, 0x1, {192, 204, 317, 323, 356, 367, 383, 386});
                createItem(iconMenu, 0x4, {55, 116, 132, 136, 138, 160, 169});
                createItem(iconMenu, 0x5, {69, 84});
                createItem(iconMenu, 0x6, {42, 77, 94, 106});
                createItem(iconMenu, 0x7, {57, 71});
                createItem(iconMenu, 0x8, {44, 59});
                createItem(iconMenu, 0x9, {41, 48, 59, 61});
                createItem(iconMenu, 0xD, {4, 20, 35});
            } else {
                iconMenu->setLayout(RowLayout::create()
                    ->setGap(24.0f)
                    ->setGrowCrossAxis(true)
                    ->setCrossAxisOverflow(false)
                    ->setCrossAxisAlignment(AxisAlignment::Center)
                    ->setCrossAxisLineAlignment(AxisAlignment::Even)
                );
                iconMenu->setContentSize({440.f, 220.f});

                createItemGroup(iconMenu, {{0x6, 106}, {0x5, 69}, {0x4, 116}, {0x1, 204}});
                createItemGroup(iconMenu, {{0x8, 59}, {0x1, 367}, {0x9, 61}, {0x4, 169}});
                createItemGroup(iconMenu, {{0x1, 356}, {0x6, 77}, {0x7, 71}, {0xD, 35}});
                createItemGroup(iconMenu, {{0x6, 42}, {0x5, 84}, {0x4, 160}, {0x1, 192}});
                createItemGroup(iconMenu, {{0xD, 4}, {0x4, 55}, {0x1, 317}, {0x9, 59}});
                createItemGroup(iconMenu, {{0x1, 383}, {0x4, 138}, {0x7, 57}, {0x9, 48}});
                createItemGroup(iconMenu, {{0x1, 386}, {0x4, 136}, {0x8, 44}, {0x9, 41}});
                createItemGroup(iconMenu, {{0xD, 20}, {0x1, 323}, {0x4, 132}, {0x6, 94}});
            }
        break;
    }
};

void TreasureListAlert::createNavButton(CCMenu *menu, int tag, bool active){
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1) ? "chest_03_02_001.png"
                : (tag == 2) ? "chest_04_02_001.png"
                : (tag == 3) ? "chest_05_02_001.png"
                : (tag == 4) ? "chest_06_02_001.png"
                : (tag == 5) ? "chest_07_02_001.png"
                : "chest_08_02_001.png";

    //  Base color based on whenever the current page is on
    auto baseColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;

    auto buttonSpr = IconSelectButtonSprite::createWithSpriteFrameName(sprName, 1.25F, baseColor);

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(TreasureListAlert::onNavButton));

    //  Adds ID and Tag
    button->setID("chest-button-" + std::to_string(tag));
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void TreasureListAlert::onNavButton(CCObject *sender){
    auto menu = this->getChildByID("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();

    m_currentPage = tag;

    auto shopTitle = (tag == 1) ? "Rewards (1x Key Chests)" 
            : (tag == 2)    ? "Rewards (5x Key Chests)"
            : (tag == 3)    ? "Rewards (10x Key Chests)"
            : (tag == 4)    ? "Rewards (25x Key Chests)"
            : (tag == 5)    ? "Rewards (50x Key Chests)"
            : "Rewards (100x Key Chests)";
    this->setTitle(shopTitle);

    navMenu->removeAllChildren();
    navMenu->updateLayout();

    for (int ii = 1; ii <= 6; ii++){
        createNavButton(navMenu, ii, tag == ii);
    };

    auto iconPage = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(tag, 1);

    /*
    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");
    nextArrow->setVisible(true);
    prevArrow->setVisible(false);
    */
};

/*
//  When the Arrow buttons are pressed (FOR THE FUTURE)
void TreasureListAlert::onPageButton(CCObject * sender){
    auto tag = sender->getTag();

    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");
    nextArrow->setVisible(tag == 1);
    prevArrow->setVisible(tag == 2);

    auto iconPage = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(m_currentPage, tag);
};*/

void TreasureListAlert::createItem(CCMenu *menu, int gamemode, std::vector<int> icons){
    for (auto const & iconID : icons){
        auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
        auto gsm = GameStatsManager::sharedState();
        UnlockType iconType{gamemode};

        auto iconSpr = GJItemIcon::createBrowserItem(
            iconType,
            iconID
        );

        if (gsm->isItemUnlocked(iconType, iconID) && !noCheckmark)
        {
            auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
            checkmark->setPosition(iconSpr->getContentSize() / 2);
            checkmark->setScale(0.75f);

            iconSpr->darkenStoreItem({30, 30, 30});
            iconSpr->addChild(checkmark);
        }
        else
        {
            if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2)
            {
                auto colorType = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
                colorType->setPosition(iconSpr->getContentSize() / 2);
                colorType->setScale(0.5f);

                iconSpr->addChild(colorType);
            }
        };

        //  Creates the Icon as a button itself.
        auto iconButton = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(TreasureListAlert::onIcon)
        );

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject(new IconParameters(gamemode, iconID, menu->getTag()));

        menu->addChild(iconButton);
        menu->updateLayout();
    }
}

void TreasureListAlert::createItemGroup(CCMenu *menu, std::map<int, int> icons) {
    log::info("Function called, map size {}", icons.size()); 

    auto auxMenu = CCMenu::create();
    auxMenu->setContentSize({80.0, 40.0});
    auxMenu->setLayout(RowLayout::create()
        ->setGap(12.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
    );

    if(icons.size() == 3){
        auxMenu->setContentSize({120.f, 40.f});
    } else if(icons.size() == 4){
        auxMenu->setContentSize({160.f, 40.f});
    }

    for (auto const & [gamemode, iconID] : icons){
        auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
        auto gsm = GameStatsManager::sharedState();
        UnlockType iconType{gamemode};

        auto iconSpr = GJItemIcon::createBrowserItem(
            iconType,
            iconID
        );

        if (gsm->isItemUnlocked(iconType, iconID) && !noCheckmark)
        {
            auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
            checkmark->setPosition(iconSpr->getContentSize() / 2);
            checkmark->setScale(0.75f);

            iconSpr->darkenStoreItem({30, 30, 30});
            iconSpr->addChild(checkmark);
        }
        else
        {
            if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2)
            {
                auto colorType = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
                colorType->setPosition(iconSpr->getContentSize() / 2);
                colorType->setScale(0.5f);

                iconSpr->addChild(colorType);
            }
        };

        //  Creates the Icon as a button itself.
        auto iconButton = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(TreasureListAlert::onIcon)
        );

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject(new IconParameters(gamemode, iconID, menu->getTag()));

        auxMenu->addChild(iconButton);
        auxMenu->updateLayout();
    }

    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
		iconListBG->setColor({ 112, 51, 31 });
        iconListBG->setContentSize({(auxMenu->getContentSize().width * 2) + 8.f, (auxMenu->getContentSize().height * 2) + 8.f});
        iconListBG->setPosition(auxMenu->getContentSize() / 2);
        iconListBG->setScale(0.5f);
        iconListBG->setZOrder(-1);
	auxMenu->addChild(iconListBG);

    menu->addChild(auxMenu);
    menu->updateLayout();
}

//	When a specific Icon Button is pressed
void TreasureListAlert::onIcon(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());
        UnlockType iconType{parameters->p_iconType};

    ItemInfoPopup::create(parameters->p_iconID, iconType)->show();
};

TreasureListAlert *TreasureListAlert::create(){
    auto ret = new TreasureListAlert();

    if (ret && ret->init(450.f, 260.f))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};