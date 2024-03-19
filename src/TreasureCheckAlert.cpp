#include "TreasureCheckAlert.hpp"

//	Icon Parameters (to create the info pop-up when pressed
struct IconParameters : public CCObject{
	UnlockType p_iconType;
	int p_iconId;

	IconParameters(UnlockType icon, int id) : p_iconType(icon), p_iconId(id){
		this->autorelease();
	}
};

//  The Mod's Popup set-up
bool TreasureCheckAlert::setup() {
    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto groupRewards = Mod::get()->getSettingValue<bool>("group-by-chest");
    loadTreasureRoomInfo();

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("1-Key Chest Rewards");

    //  Background for the Icon list
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setColor({ 133, 68, 41 });
    iconListBG->setContentSize({400, 165});
    iconListBG->setPosition({winSize.width / 2, winSize.height / 2 + 7.5f});
	this->addChild(iconListBG);

    //  Info Button menu
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({420.f, 260.f});
    infoMenu->ignoreAnchorPointForPosition(false);
    infoMenu->setID("info-menu");

    //  Info Button itself
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(TreasureCheckAlert::onInfoButton)
    );

    infoButton->setPosition(infoMenu->getContentSize() - 20.f);
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);
    this->addChild(infoMenu);

    //  Creates the menu that features buttons with the Chest sprites. (OPTIMIZED)
    auto chestMenu = CCMenu::create();
    chestMenu->setPositionY(winSize.height / 2 - 100.f);
    chestMenu->setLayout(RowLayout::create()
            ->setGap(8.f)
    );
    chestMenu->setID("chest-menu");
    this->addChild(chestMenu);

    //  Creates the buttons for the Menu. By default the first button is Selected.
    for(int ii = 1; ii <= 6; ii++){
        createChestButton(chestMenu, ii, ii == 1);
    };

    //  Creates a menu for each page available (PLANNING TO OPTIMIZE)
    auto iconPage_1 = createIconPage(1, true);
    auto iconPage_2 = createIconPage(2, false);
    auto iconPage_3 = createIconPage(3, false);
    auto iconPage_4 = createIconPage(4, false);
    auto iconPage_5 = createIconPage(5, false);
    auto iconPage_6 = createIconPage(6, false);

    /*  FOR REFERENCE
        - CUBE              = 0x1
        - COL1 (MAIN)       = 0x2
        - COL2 (SECOND)     = 0x3
        - SHIP              = 0x4
        - BALL              = 0x5
        - BIRD (UFO)        = 0x6
        - DART (WAVE)       = 0x7
        - ROBOT             = 0x8
        - SPIDER            = 0x9
        - DEATH EFFECT      = 0xB
        - SWING             = 0xD
    */

    //  Icon rewards for "1-Key" Chest
    addIcons(iconPage_1, 0x1, 97, 163, 167, 176, 179, 189, 245, 249, 255, 278, 281, 282, 285, 287, 294, 302, 313, 333, 334, 335, 348, 350, 353, 373, 391, 414, 420, 426, 428);
    addIcons(iconPage_1, 0x4, 26, 69, 83);
    addIcons(iconPage_1, 0x5, 21);
    addIcons(iconPage_1, 0x6, 19, 118);
    addIcons(iconPage_1, 0x7, 16);
    addIcons(iconPage_1, 0x8, 10);
    addIcons(iconPage_1, 0x9, 7);

    if(showMiscRewards){
        addIcons(iconPage_1, 0xB, 3);
		addIcons(iconPage_1, 0x2, 41, 101);
		addIcons(iconPage_1, 0x3, 40, 58, 61, 63, 66, 67, 69, 76, 78, 80);
    }

    //	Icon rewards for the "5-Key" Chest
    addIcons(iconPage_2, 0x1, 112, 114, 115, 116, 137, 155, 171, 175, 177, 180, 265, 269, 273, 289, 291, 322, 330);
    addIcons(iconPage_2, 0x4, 51, 73);
    addIcons(iconPage_2, 0x5, 39, 42, 58);
    addIcons(iconPage_2, 0x6, 53, 56, 97);
    addIcons(iconPage_2, 0x7, 34, 42, 76);
    addIcons(iconPage_2, 0x8, 26, 30);
    addIcons(iconPage_2, 0x9, 17);

    if (showMiscRewards)
        addIcons(iconPage_2, 0xB, 14, 15);

    //	Icon rewards for the "10-Key" Chest
    addIcons(iconPage_3, 0x1, 212, 218, 228, 256, 259, 298, 300, 318, 328, 331, 336, 338, 340, 345, 349, 355, 366, 374, 380, 382, 389, 392, 398, 402, 410, 418, 432, 437, 441, 461);
    addIcons(iconPage_3, 0x4, 64, 71, 89, 110);
    addIcons(iconPage_3, 0x5, 85, 87, 101, 109);
    addIcons(iconPage_3, 0x6, 81, 103, 123);
    addIcons(iconPage_3, 0x7, 54, 91);
    addIcons(iconPage_3, 0x8, 27, 61);
    addIcons(iconPage_3, 0x9, 34, 52, 63);
    addIcons(iconPage_3, 0xD, 14, 28, 29);

    if (showMiscRewards)
        addIcons(iconPage_3, 0x3, 64, 75, 81, 82, 83, 84, 101, 103, 105);

    if(!groupRewards){
        //	Icon rewards for the "25-Key" Chest
        addIcons(iconPage_4, 0x1, 293, 295, 404, 405, 406, 419);
        addIcons(iconPage_4, 0x4, 59, 72, 112);
        addIcons(iconPage_4, 0x5, 88, 89);
        addIcons(iconPage_4, 0x6, 36, 58, 61, 67, 99, 126);
        addIcons(iconPage_4, 0x7, 47, 73);
        addIcons(iconPage_4, 0x8, 64, 67);
        addIcons(iconPage_4, 0xD, 11, 13, 43);

        if (showMiscRewards){
            addIcons(iconPage_4, 0x2, 42, 43, 47, 50, 55, 66, 67, 68, 69, 70, 72, 73, 75, 78, 79, 81, 87, 88, 90);
            addIcons(iconPage_4, 0x3, 42, 60, 74, 98);
        }

        //	Icon rewards for the "50-Key" Chest
        addIcons(iconPage_5, 0x1, 166, 202, 261, 264, 280, 301, 361, 384, 393);
        addIcons(iconPage_5, 0x4, 82, 85, 97, 135);
        addIcons(iconPage_5, 0x5, 56, 115);
        addIcons(iconPage_5, 0x6, 87, 142, 143);
        addIcons(iconPage_5, 0x7, 79, 96);
        addIcons(iconPage_5, 0x8, 36, 50);
        addIcons(iconPage_5, 0x9, 47, 62);

        if (showMiscRewards) 
            addIcons(iconPage_5, 0x3, 44, 45, 46, 48, 50, 55, 87, 90, 92, 93, 95, 97);

        //	Icon rewards for the "100-Key" Chest
        addIcons(iconPage_6, 0x1, 192, 204, 317, 323, 356, 367, 383, 386);
        addIcons(iconPage_6, 0x4, 55, 116, 132, 136, 138, 160, 169);
        addIcons(iconPage_6, 0x5, 69, 84);
        addIcons(iconPage_6, 0x6, 42, 77, 94, 106);
        addIcons(iconPage_6, 0x7, 57, 71);
        addIcons(iconPage_6, 0x8, 44, 59);
        addIcons(iconPage_6, 0x9, 41, 48, 59, 61);
        addIcons(iconPage_6, 0xD, 4, 20, 35);
    } else {
        iconPage_4->setLayout(RowLayout::create()
            ->setGap(14.0f)
                ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_5->setLayout(RowLayout::create()
            ->setGap(21.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_6->setLayout(RowLayout::create()
            ->setGap(21.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_5->setContentSize({440.f, 220.f});
        iconPage_6->setContentSize({440.f, 220.f});

        //  Icon Rewards for the "25-Key" Chest (Grouped)
        if(showMiscRewards){
            addGroupedIcons(iconPage_4, 0x1, 293, 0x3, 98);
            addGroupedIcons(iconPage_4, 0x1, 295, 0x3, 42);
            addGroupedIcons(iconPage_4, 0x1, 404, 0x2, 70);
            addGroupedIcons(iconPage_4, 0x1, 405, 0x2, 69);
            addGroupedIcons(iconPage_4, 0x1, 406, 0x2, 68);
            addGroupedIcons(iconPage_4, 0x1, 419, 0x2, 72);
            addGroupedIcons(iconPage_4, 0x4, 59, 0x2, 78);
            addGroupedIcons(iconPage_4, 0x4, 72, 0x2, 73);
            addGroupedIcons(iconPage_4, 0x4, 112, 0x2, 43);
            addGroupedIcons(iconPage_4, 0x5, 88, 0x2, 79);
            addGroupedIcons(iconPage_4, 0x5, 89, 0x2, 50);
            addGroupedIcons(iconPage_4, 0x6, 39, 0x2, 42);
            addGroupedIcons(iconPage_4, 0x6, 58, 0x2, 75);
            addGroupedIcons(iconPage_4, 0x6, 61, 0x2, 81);
            addGroupedIcons(iconPage_4, 0x6, 67, 0x2, 55);
            addGroupedIcons(iconPage_4, 0x6, 99, 0x3, 60);
            addGroupedIcons(iconPage_4, 0x6, 126, 0x2, 87);
            addGroupedIcons(iconPage_4, 0x7, 47, 0x2, 88);
            addGroupedIcons(iconPage_4, 0x7, 73, 0x3, 74);
            addGroupedIcons(iconPage_4, 0x8, 64, 0x2, 47);
            addGroupedIcons(iconPage_4, 0x8, 67, 0x2, 46);
            addGroupedIcons(iconPage_4, 0xD, 11, 0x2, 67);
            addGroupedIcons(iconPage_4, 0xD, 13, 0x2, 66);
            addGroupedIcons(iconPage_4, 0xD, 43, 0x2, 90);
        } else {
            addIcons(iconPage_4, 0x1, 293, 295, 404, 405, 406, 419);
            addIcons(iconPage_4, 0x4, 59, 72, 112);
            addIcons(iconPage_4, 0x5, 88, 89);
            addIcons(iconPage_4, 0x6, 36, 58, 61, 67, 99, 126);
            addIcons(iconPage_4, 0x7, 47, 73);
            addIcons(iconPage_4, 0x8, 64, 67);
            addIcons(iconPage_4, 0xD, 11, 13, 43);
        }

        //  Icon Rewards for the "50-Key" Chest (Grouped)
        addGroupedIcons(iconPage_5, 0x1, 384, 0x4, 97, 0x3, 87);
        addGroupedIcons(iconPage_5, 0x1, 280, 0x6, 87, 0x3, 90);
        addGroupedIcons(iconPage_5, 0x1, 166, 0x4, 82, 0x3, 92);
        addGroupedIcons(iconPage_5, 0x5, 56, 0x9, 47, 0x3, 93);
        addGroupedIcons(iconPage_5, 0x6, 142, 0x1, 264, 0x3, 95);
        addGroupedIcons(iconPage_5, 0x7, 96, 0x4, 85, 0x3, 97);
        addGroupedIcons(iconPage_5, 0x1, 301, 0x4, 135, 0x3, 44);
        addGroupedIcons(iconPage_5, 0x7, 79, 0x1, 202, 0x3, 45);
        addGroupedIcons(iconPage_5, 0x1, 261, 0x6, 143, 0x3, 46);
        addGroupedIcons(iconPage_5, 0x8, 36, 0x5, 115, 0x3, 48);
		addGroupedIcons(iconPage_5, 0x9, 62, 0x1, 361, 0x3, 50);
		addGroupedIcons(iconPage_5, 0x8, 50, 0x1, 393, 0x3, 55);

        //  Icon Rewards for the "100-Key" Chest (Grouped)
        addGroupedIcons(iconPage_6, 0x6, 106, 0x5, 69, 0x4, 116, 0x1, 204);
		addGroupedIcons(iconPage_6, 0x8, 59, 0x1, 367, 0x9, 61, 0x4, 169);
		addGroupedIcons(iconPage_6, 0x1, 356, 0x6, 77, 0x7, 71, 0xD, 35);
		addGroupedIcons(iconPage_6, 0x6, 42, 0x5, 84, 0x4, 160, 0x1, 192);
		addGroupedIcons(iconPage_6, 0xD, 4, 0x4, 55, 0x1, 317, 0x9, 59);
		addGroupedIcons(iconPage_6, 0x1, 383, 0x4, 138, 0x7, 57, 0x9, 48);
		addGroupedIcons(iconPage_6, 0x1, 386, 0x4, 136, 0x8, 44, 0x9, 41);
		addGroupedIcons(iconPage_6, 0xD, 20, 0x1, 323, 0x4, 132, 0x6, 94);
    }
    
    //	Adds all of the Icon Pages into the main menu.
	this->addChild(iconPage_1);
    this->addChild(iconPage_2);
    this->addChild(iconPage_3);
    this->addChild(iconPage_4);
    this->addChild(iconPage_5);
    this->addChild(iconPage_6);

    this->m_noElasticity = true;
    return true;
};

void TreasureCheckAlert::loadTreasureRoomInfo(){
    auto gm = GameStatsManager::sharedState();

    for(auto ii = 0; ii < 6; ii++){
        auto offset = 1000 * ii;

        for(auto jj = 1; jj <= m_chestTotal[ii]; jj++){
            if(gm->isSecretChestUnlocked(jj + offset)){
                m_chestCount[ii]++;
            }
        }
    }
};

//	Creates the menu where every Icon is added as a button (That when pressed it displays the Info Pop-up of said Icon)
CCMenu* TreasureCheckAlert::createIconPage(int ID, bool isVisible){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    menu->setContentSize({520.f, 220.f});
    menu->setID("icon-page-" + std::to_string(ID));
    menu->setLayout(RowLayout::create()
        ->setGap(13.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
	);

    /*
    auto progress = std::to_string(m_chestCount[ID - 1]) + "/" + std::to_string(m_chestTotal[ID - 1]);
    auto text = CCLabelBMFont::create(progress.c_str(), "goldFont.fnt");
    this->addChild(text);
    */

    menu->setVisible(isVisible);
    menu->setPositionY(winSize.height / 2 + 7.5f);
    menu->setScale(0.75f);
    
    return menu;
};

//  Function to add Icons (Base of the variadic function)
template <typename T>
void TreasureCheckAlert::addIcons(CCMenu* menu, int type, const T &iconID){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{type};

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
	);

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSpr->getContentSize() / 2);
		checkmark->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
		iconSpr->addChild(checkmark);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto value = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
        value->setPosition(iconSpr->getContentSize() / 2);
		value->setScale(0.5f);

		iconSpr->addChild(value);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        menu_selector(TreasureCheckAlert::onIconButton)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();
};

//  Function to add Icons (Recursive part of the variadic function)
template <typename T, typename... ARGS>
void TreasureCheckAlert::addIcons(CCMenu* menu, int type, const T &iconID, const ARGS &...args){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{type};

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
	);

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSpr->getContentSize() / 2);
		checkmark->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
		iconSpr->addChild(checkmark);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto value = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
        value->setPosition(iconSpr->getContentSize() / 2);
		value->setScale(0.5f);

		iconSpr->addChild(value);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        menu_selector(TreasureCheckAlert::onIconButton)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();

    addIcons(menu, type, args...);
};

//  Function to add Icons by group (as how they're rewarded in the Tier II Chests)
void TreasureCheckAlert::addGroupedIcons(CCMenu* menu, int firstType, int firstID, int secondType, int secondID, int thirdType, int thirdID, int fourthType, int fourthID){
    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto auxMenu = CCMenu::create();

    auxMenu->setContentSize({80.0, 40.0});
    auxMenu->setLayout(RowLayout::create()
        ->setGap(12.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
    );

    addIcons(auxMenu, firstType, firstID);
    addIcons(auxMenu, secondType, secondID);

    //  If there's a third reward, extend the Content Size and add it.
    if(thirdType != 0x0){
        auto isCol = thirdType == 0x2 || thirdType == 0x3;

        if(!isCol || (isCol && showMiscRewards)){
            auxMenu->setContentSize({120.f, 40.f});
            addIcons(auxMenu, thirdType, thirdID);
        }
    }

    //  If there's a fourth reward, extend the Content Size and add it.
    if(fourthType != 0x0){
        auto isCol = fourthType == 0x2 || fourthType == 0x3;

        if(!isCol || (isCol && showMiscRewards)){
            auxMenu->setContentSize({150.f, 40.f});
            addIcons(auxMenu, fourthType, fourthID);
        }
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
};

//	When a specific Icon Button is pressed
void TreasureCheckAlert::onIconButton(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());

    ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
};

//  Creates a SelectButton for a specific type of Secret Chest
void TreasureCheckAlert::createChestButton(CCMenu* menu, int tag, bool isActive){
    auto baseColor = (isActive) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
    auto sprName = "chest_0" + std::to_string(tag + 2) + "_02_001.png";

    auto button = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSpriteFrameName(sprName.c_str(), 2.0F, baseColor),
        this,
        menu_selector(TreasureCheckAlert::onChestButton)
    );

    button->setID("chest-button-" + std::to_string(tag));
    button->setTag(tag);

    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navegation button is pressed
void TreasureCheckAlert::onChestButton(CCObject * sender){
    auto menu = this->getChildByID("chest-menu");
    auto btnMenu = static_cast<CCMenu*>(menu);
    auto tag = sender->getTag();

    auto type = (tag == 1) ? "1-Key Chest Rewards" : (tag == 2) ? "5-Key Chest Rewards" : (tag == 3) ? "10-Key Chest Rewards" : (tag == 4) ? "25-Key Chest Rewards" : (tag == 5) ? "50-Key Chest Rewards" : "100-Key Chest Rewards";
    this->setTitle(type);

    menu->removeAllChildren();
    menu->updateLayout();

    for(int ii = 1; ii <= 6; ii++){
        this->getChildByID("icon-page-" + std::to_string(ii))->setVisible(tag == ii);
        createChestButton(btnMenu, ii, tag == ii);
    };
};

//	When the Info Button is pressed, gives a quick summary of the Player's stats in the Treasure Room.
void TreasureCheckAlert::onInfoButton(CCObject * sender){
    auto totalKeys = (m_chestTotal[0] - m_chestCount[0]) + (m_chestTotal[1] - m_chestCount[1]) * 5 + (m_chestTotal[2] - m_chestCount[2]) * 10 + (m_chestTotal[3] - m_chestCount[3]) * 25 + (m_chestTotal[4] - m_chestCount[4]) * 50 + (m_chestTotal[5] - m_chestCount[5]) * 100;

    std::string info =
        "<cj>1-Key Chests:</c> " + std::to_string(m_chestCount[0]) + " out of " + std::to_string(m_chestTotal[0]) +
        "\n<cg>5-Key Chests:</c> " + std::to_string(m_chestCount[1]) + " out of " + std::to_string(m_chestTotal[1]) +
        "\n<cy>10-Key Chests:</c> " + std::to_string(m_chestCount[2]) + " out of " + std::to_string(m_chestTotal[2]) +
        "\n<co>25-Key Chests:</c> " + std::to_string(m_chestCount[3]) + " out of " + std::to_string(m_chestTotal[3]) +
        "\n<cr>50-Key Chests:</c> " + std::to_string(m_chestCount[4]) + " out of " + std::to_string(m_chestTotal[4]) +
        "\n<cp>100-Key Chests:</c> " + std::to_string(m_chestCount[5]) + " out of " + std::to_string(m_chestTotal[5]) +
        "\n\nKeys needed to 100%: " + std::to_string(totalKeys);

	FLAlertLayer::create("Treasure Room Stats", info.c_str(), "OK")->show();
};

TreasureCheckAlert * TreasureCheckAlert::create() {
    auto ret = new TreasureCheckAlert();

    if (ret && ret->init(420.f, 260.f)){
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};