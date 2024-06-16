#include "TreasureCheckAlert.hpp"

//	Icon Parameters (to create the info pop-up when pressed
struct IconParameters : public CCObject{
	UnlockType p_iconType;
	int p_iconId;

	/// @brief 
	/// @param icon 
	/// @param id 

	IconParameters(UnlockType icon, int id) : p_iconType(icon), p_iconId(id){
		this->autorelease();
	}
};

//  The set-up for the Mod's popup
bool TreasureCheckAlert::setup() {
    //  loadInfo();

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("Rewards from 1x Key Chests");

    //  Background for the Icon list
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({winSize.width / 2, winSize.height / 2 + 8.0f});
    iconListBG->setColor({ 133, 68, 41 });
    iconListBG->setContentSize({400, 165});
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
    //  this->addChild(infoMenu);

    //  Creates the menu that features buttons with the Chest sprites. (OPTIMIZED)
    auto pageMenu = CCMenu::create();
    pageMenu->setPositionY(winSize.height / 2 - 100.f);
    pageMenu->setLayout(RowLayout::create()
            ->setGap(8.f)
    );
    pageMenu->setID("chest-menu");
    this->addChild(pageMenu);

    //  Creates the buttons for the Menu. By default the first button is Selected.
    for(int ii = 1; ii <= 6; ii++){
        createPageButton(pageMenu, ii, ii == 1);
    };
    /*
    for(int ii = 1; ii <= 6; ii++){
        createPageButton(pageMenu, ii, ii == 1);
    };

    for(int ii = 0; ii < m_chestTotal.size(); ii++){
        createPageButton(pageMenu, ii + 1, ii == 0);
    };
    */

    //  Creates a menu for each page available (PLANNING TO OPTIMIZE)
    CCSize defaultSize = {520.f, 220.f};

    auto iconPage_1 = createIconMenu(defaultSize, 1, true);
    auto iconPage_2 = createIconMenu(defaultSize, 2, false);
    auto iconPage_3 = createIconMenu(defaultSize, 3, false);
    auto iconPage_4 = createIconMenu(defaultSize, 4, false);
    auto iconPage_5 = createIconMenu(defaultSize, 5, false);
    auto iconPage_6 = createIconMenu(defaultSize, 6, false);

    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto groupRewards = Mod::get()->getSettingValue<bool>("group-by-chest");

    //  Icon rewards for "1-Key" Chest
    addIcons(iconPage_1, rewardType::cube, 97, 163, 167, 176, 179, 189, 245, 249, 255, 278, 281, 282, 285, 287, 294, 302, 313, 333, 334, 335, 348, 350, 353, 373, 391, 414, 420, 426, 428);
    addIcons(iconPage_1, rewardType::ship, 26, 69, 83);
    addIcons(iconPage_1, rewardType::ball, 21);
    addIcons(iconPage_1, rewardType::bird, 19, 118);
    addIcons(iconPage_1, rewardType::dart, 16);
    addIcons(iconPage_1, rewardType::robot, 10);
    addIcons(iconPage_1, rewardType::spider, 7);

    if(showMiscRewards){
        addIcons(iconPage_1, rewardType::effect, 3);
		addIcons(iconPage_1, rewardType::col1, 41, 101);
		addIcons(iconPage_1, rewardType::col2, 40, 58, 61, 63, 66, 67, 69, 76, 78, 80);
    }

    //	Icon rewards for the "5-Key" Chest
    addIcons(iconPage_2, rewardType::cube, 112, 114, 115, 116, 137, 155, 171, 175, 177, 180, 265, 269, 273, 289, 291, 322, 330);
    addIcons(iconPage_2, rewardType::ship, 51, 73);
    addIcons(iconPage_2, rewardType::ball, 39, 42, 58);
    addIcons(iconPage_2, rewardType::bird, 53, 56, 97);
    addIcons(iconPage_2, rewardType::dart, 34, 42, 76);
    addIcons(iconPage_2, rewardType::robot, 26, 30);
    addIcons(iconPage_2, rewardType::spider, 17);

    if (showMiscRewards)
        addIcons(iconPage_2, rewardType::effect, 14, 15);

    //	Icon rewards for the "10-Key" Chest
    addIcons(iconPage_3, rewardType::cube, 212, 218, 228, 256, 259, 298, 300, 318, 328, 331, 336, 338, 340, 345, 349, 355, 366, 374, 380, 382, 389, 392, 398, 402, 410, 418, 432, 437, 441, 461);
    addIcons(iconPage_3, rewardType::ship, 64, 71, 89, 110);
    addIcons(iconPage_3, rewardType::ball, 85, 87, 101, 109);
    addIcons(iconPage_3, rewardType::bird, 81, 103, 123);
    addIcons(iconPage_3, rewardType::dart, 54, 91);
    addIcons(iconPage_3, rewardType::robot, 27, 61);
    addIcons(iconPage_3, rewardType::spider, 34, 52, 63);
    addIcons(iconPage_3, rewardType::swing, 14, 28, 29);

    if (showMiscRewards)
        addIcons(iconPage_3, rewardType::col2, 64, 75, 81, 82, 83, 84, 101, 103, 105);

    if(!groupRewards){
        //	Icon rewards for the "25-Key" Chest
        addIcons(iconPage_4, rewardType::cube, 293, 295, 404, 405, 406, 419);
        addIcons(iconPage_4, rewardType::ship, 59, 72, 112);
        addIcons(iconPage_4, rewardType::ball, 88, 89);
        addIcons(iconPage_4, rewardType::bird, 36, 58, 61, 67, 99, 126);
        addIcons(iconPage_4, rewardType::dart, 47, 73);
        addIcons(iconPage_4, rewardType::robot, 64, 67);
        addIcons(iconPage_4, rewardType::swing, 11, 13, 43);

        if (showMiscRewards){
            addIcons(iconPage_4, rewardType::col1, 42, 43, 47, 50, 55, 66, 67, 68, 69, 70, 72, 73, 75, 78, 79, 81, 87, 88, 90);
            addIcons(iconPage_4, rewardType::col2, 42, 60, 74, 98);
        }

        //	Icon rewards for the "50-Key" Chest
        addIcons(iconPage_5, rewardType::cube, 166, 202, 261, 264, 280, 301, 361, 384, 393);
        addIcons(iconPage_5, rewardType::ship, 82, 85, 97, 135);
        addIcons(iconPage_5, rewardType::ball, 56, 115);
        addIcons(iconPage_5, rewardType::bird, 87, 142, 143);
        addIcons(iconPage_5, rewardType::dart, 79, 96);
        addIcons(iconPage_5, rewardType::robot, 36, 50);
        addIcons(iconPage_5, rewardType::spider, 47, 62);

        if (showMiscRewards) 
            addIcons(iconPage_5, rewardType::col2, 44, 45, 46, 48, 50, 55, 87, 90, 92, 93, 95, 97);

        //	Icon rewards for the "100-Key" Chest
        addIcons(iconPage_6, rewardType::cube, 192, 204, 317, 323, 356, 367, 383, 386);
        addIcons(iconPage_6, rewardType::ship, 55, 116, 132, 136, 138, 160, 169);
        addIcons(iconPage_6, rewardType::ball, 69, 84);
        addIcons(iconPage_6, rewardType::bird, 42, 77, 94, 106);
        addIcons(iconPage_6, rewardType::dart, 57, 71);
        addIcons(iconPage_6, rewardType::robot, 44, 59);
        addIcons(iconPage_6, rewardType::spider, 41, 48, 59, 61);
        addIcons(iconPage_6, rewardType::swing, 4, 20, 35);
    } else {
        iconPage_4->setLayout(RowLayout::create()
            ->setGap(14.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_5->setLayout(RowLayout::create()
            ->setGap(24.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_6->setLayout(RowLayout::create()
            ->setGap(24.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Even)
        );

        iconPage_5->setContentSize({440.f, 220.f});
        iconPage_6->setContentSize({440.f, 220.f});

        //  Icon Rewards for the "25-Key" Chest (Grouped)
        if(showMiscRewards){
            addIconGroup(iconPage_4, rewardType::cube, 293, rewardType::col2, 98);
            addIconGroup(iconPage_4, rewardType::cube, 295, rewardType::col2, 42);
            addIconGroup(iconPage_4, rewardType::cube, 404, rewardType::col1, 70);
            addIconGroup(iconPage_4, rewardType::cube, 405, rewardType::col1, 69);
            addIconGroup(iconPage_4, rewardType::cube, 406, rewardType::col1, 68);
            addIconGroup(iconPage_4, rewardType::cube, 419, rewardType::col1, 72);
            addIconGroup(iconPage_4, rewardType::ship, 59, rewardType::col1, 78);
            addIconGroup(iconPage_4, rewardType::ship, 72, rewardType::col1, 73);
            addIconGroup(iconPage_4, rewardType::ship, 112, rewardType::col1, 43);
            addIconGroup(iconPage_4, rewardType::ball, 88, rewardType::col1, 79);
            addIconGroup(iconPage_4, rewardType::ball, 89, rewardType::col1, 50);
            addIconGroup(iconPage_4, rewardType::bird, 39, rewardType::col1, 42);
            addIconGroup(iconPage_4, rewardType::bird, 58, rewardType::col1, 75);
            addIconGroup(iconPage_4, rewardType::bird, 61, rewardType::col1, 81);
            addIconGroup(iconPage_4, rewardType::bird, 67, rewardType::col1, 55);
            addIconGroup(iconPage_4, rewardType::bird, 99, rewardType::col2, 60);
            addIconGroup(iconPage_4, rewardType::bird, 126, rewardType::col1, 87);
            addIconGroup(iconPage_4, rewardType::dart, 47, rewardType::col1, 88);
            addIconGroup(iconPage_4, rewardType::dart, 73, rewardType::col2, 74);
            addIconGroup(iconPage_4, rewardType::robot, 64, rewardType::col1, 47);
            addIconGroup(iconPage_4, rewardType::robot, 67, rewardType::col1, 46);
            addIconGroup(iconPage_4, rewardType::swing, 11, rewardType::col1, 67);
            addIconGroup(iconPage_4, rewardType::swing, 13, rewardType::col1, 66);
            addIconGroup(iconPage_4, rewardType::swing, 43, rewardType::col1, 90);
        } else {
            addIcons(iconPage_4, rewardType::cube, 293, 295, 404, 405, 406, 419);
            addIcons(iconPage_4, rewardType::ship, 59, 72, 112);
            addIcons(iconPage_4, rewardType::ball, 88, 89);
            addIcons(iconPage_4, rewardType::bird, 36, 58, 61, 67, 99, 126);
            addIcons(iconPage_4, rewardType::dart, 47, 73);
            addIcons(iconPage_4, rewardType::robot, 64, 67);
            addIcons(iconPage_4, rewardType::swing, 11, 13, 43);
        }

        //  Icon Rewards for the "50-Key" Chest (Grouped)
        addIconGroup(iconPage_5, rewardType::cube, 384, rewardType::ship, 97, rewardType::col2, 87);
        addIconGroup(iconPage_5, rewardType::cube, 280, rewardType::bird, 87, rewardType::col2, 90);
        addIconGroup(iconPage_5, rewardType::cube, 166, rewardType::ship, 82, rewardType::col2, 92);
        addIconGroup(iconPage_5, rewardType::ball, 56, rewardType::spider, 47, rewardType::col2, 93);
        addIconGroup(iconPage_5, rewardType::bird, 142, rewardType::cube, 264, rewardType::col2, 95);
        addIconGroup(iconPage_5, rewardType::dart, 96, rewardType::ship, 85, rewardType::col2, 97);
        addIconGroup(iconPage_5, rewardType::cube, 301, rewardType::ship, 135, rewardType::col2, 44);
        addIconGroup(iconPage_5, rewardType::dart, 79, rewardType::cube, 202, rewardType::col2, 45);
        addIconGroup(iconPage_5, rewardType::cube, 261, rewardType::bird, 143, rewardType::col2, 46);
        addIconGroup(iconPage_5, rewardType::robot, 36, rewardType::ball, 115, rewardType::col2, 48);
		addIconGroup(iconPage_5, rewardType::spider, 62, rewardType::cube, 361, rewardType::col2, 50);
		addIconGroup(iconPage_5, rewardType::robot, 50, rewardType::cube, 393, rewardType::col2, 55);

        //  Icon Rewards for the "100-Key" Chest (Grouped)
        addIconGroup(iconPage_6, rewardType::bird, 106, rewardType::ball, 69, rewardType::ship, 116, rewardType::cube, 204);
		addIconGroup(iconPage_6, rewardType::robot, 59, rewardType::cube, 367, rewardType::spider, 61, rewardType::ship, 169);
		addIconGroup(iconPage_6, rewardType::cube, 356, rewardType::bird, 77, rewardType::dart, 71, rewardType::swing, 35);
		addIconGroup(iconPage_6, rewardType::bird, 42, rewardType::ball, 84, rewardType::ship, 160, rewardType::cube, 192);
		addIconGroup(iconPage_6, rewardType::swing, 4, rewardType::ship, 55, rewardType::cube, 317, rewardType::spider, 59);
		addIconGroup(iconPage_6, rewardType::cube, 383, rewardType::ship, 138, rewardType::dart, 57, rewardType::spider, 48);
		addIconGroup(iconPage_6, rewardType::cube, 386, rewardType::ship, 136, rewardType::robot, 44, rewardType::spider, 41);
		addIconGroup(iconPage_6, rewardType::swing, 20, rewardType::cube, 323, rewardType::ship, 132, rewardType::bird, 94);
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

void TreasureCheckAlert::loadInfo(){
    auto gm = GameStatsManager::sharedState();

    /*
    for(auto ii = 0; ii < 6; ii++){
        auto offset = 1000 * ii;

        for(auto jj = 1; jj <= m_chestTotal[ii]; jj++){
            if(gm->isSecretChestUnlocked(jj + offset)){
                m_chestCount[ii]++;
            }
        }
    }
    */
};

//	Creates the menu where every Icon is added as a button (That when pressed it displays the Info Pop-up of said Icon)
CCMenu * TreasureCheckAlert::createIconMenu(CCSize size, int ID, bool isVisible){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    menu->setContentSize(size);
    menu->setID("icon-page-" + std::to_string(ID));
    menu->setLayout(RowLayout::create()
        ->setGap(14.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
    );

    menu->setPositionY(winSize.height / 2 + 8.f);
    menu->setVisible(isVisible);
    menu->setScale(0.75f);

    return menu;
};

//  Creates a SelectButton for a specific type of Secret Chest
void TreasureCheckAlert::createPageButton(CCMenu* menu, int tag, bool isActive){
    auto baseColor = (isActive) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
    auto sprName = "chest_0" + std::to_string(tag + 2) + "_02_001.png";

    auto button = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSpriteFrameName(sprName.c_str(), 1.5F, baseColor),
        this,
        menu_selector(TreasureCheckAlert::onPageButton)
    );

    button->setID("chest-button-" + std::to_string(tag));
    button->setTag(tag);

    menu->addChild(button);
    menu->updateLayout();
};

//  Function to add Icons (Base of the variadic function)
template <typename T>
void TreasureCheckAlert::addIcons(CCMenu* menu, rewardType type, const T &iconID){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{(int)type};

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
        menu_selector(TreasureCheckAlert::onIconClick)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();
};

//  Function to add Icons (Recursive part of the variadic function)
template <typename T, typename... ARGS>
void TreasureCheckAlert::addIcons(CCMenu* menu, rewardType type, const T &iconID, const ARGS &...args){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{(int)type};

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
        menu_selector(TreasureCheckAlert::onIconClick)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();

    addIcons(menu, type, args...);
};

//  Function to add Icons by group (as how they're rewarded in the Tier II Chests)
void TreasureCheckAlert::addIconGroup(CCMenu* menu, rewardType firstType, int firstID, rewardType secondType, int secondID, rewardType thirdType, int thirdID, rewardType fourthType, int fourthID){
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
    if(thirdID != 0){
        auto isCol = thirdType == rewardType::col1 || thirdType == rewardType::col2;

        if(!isCol || (isCol && showMiscRewards)){
            auxMenu->setContentSize({120.f, 40.f});
            addIcons(auxMenu, thirdType, thirdID);
        }
    }

    //  If there's a fourth reward, extend the Content Size and add it.
    if(fourthID != 0){
        auto isCol = fourthType == rewardType::col1 || fourthType == rewardType::col2;

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
void TreasureCheckAlert::onIconClick(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());

    ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
};

//  When a Navegation button is pressed
void TreasureCheckAlert::onPageButton(CCObject * sender){
    auto menu = this->getChildByID("chest-menu");
    auto btnMenu = static_cast<CCMenu*>(menu);
    auto tag = sender->getTag();

    auto type = (tag == 1) ? "Rewards from 1x Key Chests" : (tag == 2) ? "Rewards from 5x Key Chests" : (tag == 3) ? "Rewards from 10x Key Chests" : (tag == 4) ? "Rewards from 25x Key Chests" : (tag == 5) ? "Rewards from 50x Key Chests" : "Rewards from 100x Key Chests";
    this->setTitle(type);

    menu->removeAllChildren();
    menu->updateLayout();

    for(int ii = 1; ii <= 6; ii++){
        this->getChildByID("icon-page-" + std::to_string(ii))->setVisible(tag == ii);
        createPageButton(btnMenu, ii, tag == ii);
    };
};

//	When the Info Button is pressed, gives a quick summary of the Player's stats in the Treasure Room.
void TreasureCheckAlert::onInfoButton(CCObject * sender){
    auto totalKeys = (m_chestTotal[0] - m_chestCount[0]) + (m_chestTotal[1] - m_chestCount[1]) * 5 + (m_chestTotal[2] - m_chestCount[2]) * 10 + (m_chestTotal[3] - m_chestCount[3]) * 25 + (m_chestTotal[4] - m_chestCount[4]) * 50 + (m_chestTotal[5] - m_chestCount[5]) * 100;

    std::string info =
        "<cj>1x Key Chests:</c> " + std::to_string(m_chestCount[0]) + " out of " + std::to_string(m_chestTotal[0]) +
        "\n<cg>5x Key Chests:</c> " + std::to_string(m_chestCount[1]) + " out of " + std::to_string(m_chestTotal[1]) +
        "\n<cy>10x Key Chests:</c> " + std::to_string(m_chestCount[2]) + " out of " + std::to_string(m_chestTotal[2]) +
        "\n<co>25x Key Chests:</c> " + std::to_string(m_chestCount[3]) + " out of " + std::to_string(m_chestTotal[3]) +
        "\n<cr>50x Key Chests:</c> " + std::to_string(m_chestCount[4]) + " out of " + std::to_string(m_chestTotal[4]) +
        "\n<cp>100x Key Chests:</c> " + std::to_string(m_chestCount[5]) + " out of " + std::to_string(m_chestTotal[5]) +
        "\nKeys needed to 100%: " + std::to_string(totalKeys);
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