#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;

//	Offset for the Chest IDs
enum chestTypeIDs
{
	chest05 = 1000,
	chest10 = 2000,
	chest25 = 3000,
	chest50 = 4000,
	chest100 = 5000
};

//	Icon Parameters (to create the info pop-up when pressed
struct IconParameters : public CCObject
{
	UnlockType p_iconType;
	int p_iconId;

	IconParameters(UnlockType icon, int id) : p_iconType(icon), p_iconId(id)
	{
		this->autorelease();
	}
};

//	The Mod's Custom FLAlert
class TreasureChecklistAlert : public geode::Popup<>
{
protected:
	bool setup() override
	{
		//	Gets the value of the Mod's settings
		auto showOtherRewards = Mod::get()->getSettingValue<bool>("misc-rewards");

		auto winSize = CCDirector::sharedDirector()->getWinSize();
		this->setTitle("1-Key Chest Rewards");

		auto iconListBG = CCScale9Sprite::create(
			"square02b_001.png",
			{ 0, 0, 80, 80 }
		);
		iconListBG->setOpacity(50);
		iconListBG->setColor({ 0, 0, 0 });
		iconListBG->setContentSize({400, 170});
		iconListBG->setPosition({winSize.width / 2, winSize.height / 2 + 7.5f});
		this->addChild(iconListBG);

		//	For the Info Button at the top-right corner.
		auto infoMenu = CCMenu::create();
		infoMenu->setContentSize({420.f, 260.f});
		infoMenu->ignoreAnchorPointForPosition(false);

		//	The button itself
		auto infoButton = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
			this,
			menu_selector(TreasureChecklistAlert::onInfoButton));

		infoButton->setPosition(infoMenu->getContentSize() - 20.f);
		infoMenu->addChild(infoButton);
		this->addChild(infoMenu);

		/*
			NOTE: I know this code might be hella copy+paste, I am figuring out how to optimize all this mess later.
		*/

		//	Creates the menu that features the buttons with the Chest sprites. Each page going to a specific type of Secret Chest (PLANNED TO OPTIMIZE)
		createChestMenu("menu-page-01", 1, true);
		createChestMenu("menu-page-02", 2, false);
		createChestMenu("menu-page-03", 3, false);
		createChestMenu("menu-page-04", 4, false);
		createChestMenu("menu-page-05", 5, false);
		createChestMenu("menu-page-06", 6, false);

		//	For each instruction it creates a menu for the Icon rewards of each type of Secret Chest (PLANNED TO OPTIMIZE)
		auto iconPage01 = createIconPage("icon-page-01", true);
		auto iconPage02 = createIconPage("icon-page-02", false);
		auto iconPage03 = createIconPage("icon-page-03", false);
		auto iconPage04 = createIconPage("icon-page-04", false);
		auto iconPage05 = createIconPage("icon-page-05", false);
		auto iconPage06 = createIconPage("icon-page-06", false);

		//	Icon rewards for the "1-Key" Chest
		addIcons(iconPage01, UnlockType::Cube, 97, 163, 167, 176, 179, 189, 245, 249, 255, 278, 281, 282, 285, 287, 294, 302, 313, 333, 334, 335, 348, 350, 353, 373, 391, 414, 420, 426);
		addIcons(iconPage01, UnlockType::Ship, 26, 69, 83);
		addIcons(iconPage01, UnlockType::Ball, 21);
		addIcons(iconPage01, UnlockType::Bird, 19, 118);
		addIcons(iconPage01, UnlockType::Dart, 16);
		addIcons(iconPage01, UnlockType::Robot, 10);
		addIcons(iconPage01, UnlockType::Spider, 7);

		if (showOtherRewards)
		{
			addIcons(iconPage01, UnlockType::Death, 3);
			addIcons(iconPage01, UnlockType::Col1, 41, 101);
			addIcons(iconPage01, UnlockType::Col2, 40, 55, 61, 63, 66, 67, 69, 76, 78, 80);
		}

		//	Icon rewards for the "5-Key" Chest
		addIcons(iconPage02, UnlockType::Cube, 112, 114, 115, 116, 137, 155, 171, 175, 177, 180, 265, 269, 273, 289, 291, 322, 330);
		addIcons(iconPage02, UnlockType::Ship, 51, 73);
		addIcons(iconPage02, UnlockType::Ball, 39, 42, 58);
		addIcons(iconPage02, UnlockType::Bird, 53, 56, 97);
		addIcons(iconPage02, UnlockType::Dart, 34, 42, 76);
		addIcons(iconPage02, UnlockType::Robot, 26, 30);
		addIcons(iconPage02, UnlockType::Spider, 17);

		if (showOtherRewards)
			addIcons(iconPage02, UnlockType::Death, 14, 15);

		//	Icon rewards for the "10-Key" Chest
		addIcons(iconPage03, UnlockType::Cube, 212, 218, 228, 256, 259, 298, 300, 318, 328, 331, 336, 338, 340, 345, 349, 355, 366, 374, 380, 382, 389, 392, 398, 402, 410, 418, 432, 437, 441, 461);
		addIcons(iconPage03, UnlockType::Ship, 64, 71, 89, 110);
		addIcons(iconPage03, UnlockType::Ball, 85, 87, 101, 109);
		addIcons(iconPage03, UnlockType::Bird, 81, 103, 123);
		addIcons(iconPage03, UnlockType::Dart, 54, 91);
		addIcons(iconPage03, UnlockType::Robot, 27, 61);
		addIcons(iconPage03, UnlockType::Spider, 34, 52, 63);
		addIcons(iconPage03, UnlockType::Swing, 14, 28, 29);

		if (showOtherRewards)
			addIcons(iconPage03, UnlockType::Col2, 64, 75, 81, 82, 83, 84, 101, 103, 105);

		//	Icon rewards for the "25-Key" Chest
		addIcons(iconPage04, UnlockType::Cube, 293, 295, 404, 405, 406, 419);
		addIcons(iconPage04, UnlockType::Ship, 59, 72, 112);
		addIcons(iconPage04, UnlockType::Ball, 88, 89);
		addIcons(iconPage04, UnlockType::Bird, 36, 58, 61, 67, 99, 126);
		addIcons(iconPage04, UnlockType::Dart, 47, 73);
		addIcons(iconPage04, UnlockType::Robot, 64, 67);
		addIcons(iconPage04, UnlockType::Swing, 11, 13, 43);

		if (showOtherRewards)
		{
			addIcons(iconPage04, UnlockType::Col1, 42, 43, 47, 50, 55, 66, 67, 68, 69, 70, 72, 73, 75, 78, 79, 81, 87, 88, 90);
			addIcons(iconPage04, UnlockType::Col2, 42, 60, 74, 98);
		}

		//	Icon rewards for the "50-Key" Chest
		addIcons(iconPage05, UnlockType::Cube, 166, 202, 261, 264, 280, 301, 361, 384, 393);
		addIcons(iconPage05, UnlockType::Ship, 82, 85, 97, 135);
		addIcons(iconPage05, UnlockType::Ball, 56, 115);
		addIcons(iconPage05, UnlockType::Bird, 87, 142, 143);
		addIcons(iconPage05, UnlockType::Dart, 79, 96);
		addIcons(iconPage05, UnlockType::Robot, 36, 50);
		addIcons(iconPage05, UnlockType::Spider, 47, 62);

		if (showOtherRewards)
		{
			addIcons(iconPage05, UnlockType::Col2, 44, 45, 46, 48, 50, 55, 87, 90, 92, 93, 95, 97);
		}

		//	Icon rewards for the "100-Key" Chest
		addIcons(iconPage06, UnlockType::Cube, 192, 204, 317, 323, 356, 367, 383, 386);
		addIcons(iconPage06, UnlockType::Ship, 55, 116, 132, 136, 138, 160, 169);
		addIcons(iconPage06, UnlockType::Ball, 69, 84);
		addIcons(iconPage06, UnlockType::Bird, 42, 77, 94, 106);
		addIcons(iconPage06, UnlockType::Dart, 57, 71);
		addIcons(iconPage06, UnlockType::Robot, 44, 59);
		addIcons(iconPage06, UnlockType::Spider, 41, 48, 59, 61);
		addIcons(iconPage06, UnlockType::Swing, 4, 20, 35);

		//	Adds all of the Icon Pages into the main menu.
		this->addChild(iconPage01);
		this->addChild(iconPage02);
		this->addChild(iconPage03);
		this->addChild(iconPage04);
		this->addChild(iconPage05);
		this->addChild(iconPage06);

		return true;
	}

	//	Creates the menu that features the Chest Type buttons (Called multiple times)
	void createChestMenu(const char *ID, int tag, bool visible)
	{
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto menu = CCMenu::create();

		menu->setID(ID);
		menu->setLayout(RowLayout::create()
			->setGap(8.0f));

		//	The tag is passed as parameter, so when the value is equal to the 
		createChestButton(menu, "chest_03_02_001.png", 1, tag);
		createChestButton(menu, "chest_04_02_001.png", 2, tag);
		createChestButton(menu, "chest_05_02_001.png", 3, tag);
		createChestButton(menu, "chest_06_02_001.png", 4, tag);
		createChestButton(menu, "chest_07_02_001.png", 5, tag);
		createChestButton(menu, "chest_08_02_001.png", 6, tag);

		menu->setPositionY(winSize.height / 2 - 100.f);
		menu->setVisible(visible);

		this->addChild(menu);
	};
	
	//	Creates a button for a specific type of Secret Chest (Called multiple times but with different parameters)
	void createChestButton(CCMenu *menu, const char *spriteName, int tag, int menuTag)
	{
		//	If the value of the tag equals to the tag of the Menu (Passed as parameter from the createChestMenu() function), it changes the Base Color of the button
		auto value = (tag == menuTag) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
		
		auto spr = IconSelectButtonSprite::createWithSpriteFrameName(spriteName, 2.f, value);
		auto button = CCMenuItemSpriteExtra::create(
			spr,
			this,
			menu_selector(TreasureChecklistAlert::onChestButton));

		//	Passing parameter for the Callback
		button->setTag(tag);

		button->setID("chest-button-0" + std::to_string(tag));
		menu->addChild(button);
		menu->updateLayout();
	}

	//	Creates the menu where every Icon is added as a button (That when pressed it displays the Info Pop-up of said Icon)
	CCMenu *createIconPage(const char *ID, bool visible)
	{
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto page = CCMenu::create();

		page->setContentSize({500.0, 120.0});
		page->setID(ID);
		page->setLayout(RowLayout::create()
			->setGap(13.0f)
			->setGrowCrossAxis(true)
			->setCrossAxisLineAlignment(AxisAlignment::Even));
		page->setVisible(visible);
		page->setPositionY(winSize.height / 2 + 7.5f);
		page->setScale(0.775f);

		return page;
	};

	//	EXPERIMENTAL: Using a Variadic function to add Icons per one gamemode, where unknown amount of IDs is passed as parameters
	template <typename T>
	void addIcons(CCMenu *menu, UnlockType iconType, const T &iconId)
	{
		auto withoutCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto gm = GameStatsManager::sharedState();

		auto iconSpr = GJItemIcon::createBrowserItem(
			iconType,
			iconId);

		//	If the Player has unlocked that icon, displays it with a Checkmark.
		if (gm->isItemUnlocked(iconType, iconId) && !withoutCheckmark)
		{
			iconSpr->changeToLockedState(0.0);

			auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
			checkmark->setPosition(iconSpr->getContentSize() / 2);
			checkmark->setScale(0.75f);

			iconSpr->addChild(checkmark);
		} else if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
			auto text = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
			text->setPosition(iconSpr->getContentSize() / 2);
			text->setScale(0.5f);
			
			iconSpr->addChild(text);
		};

		auto iconButton = CCMenuItemSpriteExtra::create(
			iconSpr,
			this,
			menu_selector(TreasureChecklistAlert::onIconButton));

		//	Passes the user-defined IconParameters for its callback
		iconButton->setUserObject(new IconParameters(iconType, iconId));
		
		menu->addChild(iconButton);
		menu->updateLayout();
	};

	template <typename T, typename... ARGS>
	void addIcons(CCMenu *menu, UnlockType iconType, const T &iconId, const ARGS &...args)
	{
		auto withoutCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto gm = GameStatsManager::sharedState();

		auto iconSpr = GJItemIcon::createBrowserItem(
			iconType,
			iconId);

		if (gm->isItemUnlocked(iconType, iconId) && !withoutCheckmark)
		{
			iconSpr->changeToLockedState(0.0);

			auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
			checkmark->setPosition(iconSpr->getContentSize() / 2);
			checkmark->setScale(0.75f);

			iconSpr->addChild(checkmark);
		} else if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
			auto text = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
			text->setPosition(iconSpr->getContentSize() / 2);
			text->setScale(0.5f);
			
			iconSpr->addChild(text);
		};

		auto iconButton = CCMenuItemSpriteExtra::create(
			iconSpr,
			this,
			menu_selector(TreasureChecklistAlert::onIconButton));

		iconButton->setUserObject(new IconParameters(iconType, iconId));
		menu->addChild(iconButton);
		menu->updateLayout();

		addIcons(menu, iconType, args...);
	};

	//	When a Chest button is pressed.
	void onChestButton(CCObject *sender)
	{
		/*
			TODO: Optimize this code later.
		*/
		this->getChildByID("menu-page-01")->setVisible(false);
		this->getChildByID("menu-page-02")->setVisible(false);
		this->getChildByID("menu-page-03")->setVisible(false);
		this->getChildByID("menu-page-04")->setVisible(false);
		this->getChildByID("menu-page-05")->setVisible(false);
		this->getChildByID("menu-page-06")->setVisible(false);

		this->getChildByID("icon-page-01")->setVisible(false);
		this->getChildByID("icon-page-02")->setVisible(false);
		this->getChildByID("icon-page-03")->setVisible(false);
		this->getChildByID("icon-page-04")->setVisible(false);
		this->getChildByID("icon-page-05")->setVisible(false);
		this->getChildByID("icon-page-06")->setVisible(false);

		auto tag = sender->getTag();
		auto menu = this->getChildByID("menu-page-0" + std::to_string(tag));
		menu->setVisible(true);

		auto page = this->getChildByID("icon-page-0" + std::to_string(tag));
		page->setVisible(true);

		/*
			TODO: Optimize or write better code for this.
		*/
		switch (tag)
		{
		case 1:
			this->setTitle("1-Key Chest Rewards");
			break;
		case 2:
			this->setTitle("5-Key Chest Rewards");
			break;
		case 3:
			this->setTitle("10-Key Chest Rewards");
			break;
		case 4:
			this->setTitle("25-Key Chest Rewards");
			break;
		case 5:
			this->setTitle("50-Key Chest Rewards");
			break;
		case 6:
			this->setTitle("100-Key Chest Rewards");
			break;
		}
	};

	//	When a specific Icon Button is pressed
	void onIconButton(CCObject *sender)
	{
		auto parameters = static_cast<IconParameters *>(
			static_cast<CCNode *>(sender)->getUserObject());

		ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
	};

	//	When the Info Button is pressed, gives a quick summary of the Player's stats in the Treasure Room.
	void onInfoButton(CCObject *)
	{
		CCDictionary *treasureRoomData = GameStatsManager::sharedState()->m_treasureRoomChests;
		int chestTotal[] = {400, 100, 60, 24, 12, 8};
		int chestCount[] = {0, 0, 0, 0, 0, 0};

		CCDictElement *chestObject;
		CCDICT_FOREACH(treasureRoomData, chestObject)
		{
			const int chestID = std::stoi(chestObject->getStrKey());

			if (chestID < chestTypeIDs::chest05)
			{
				chestCount[0]++;
			}
			else if (chestID < chestTypeIDs::chest10)
			{
				chestCount[1]++;
			}
			else if (chestID < chestTypeIDs::chest25)
			{
				chestCount[2]++;
			}
			else if (chestID < chestTypeIDs::chest50)
			{
				chestCount[3]++;
			}
			else if (chestID < chestTypeIDs::chest100)
			{
				chestCount[4]++;
			}
			else
			{
				chestCount[5]++;
			}
		}

		const int keysNeeded = (chestTotal[0] - chestCount[0]) + (chestTotal[1] - chestCount[1]) * 5 + (chestTotal[2] - chestCount[2]) * 10 + (chestTotal[3] - chestCount[3]) * 25 + (chestTotal[4] - chestCount[4]) * 50 + (chestTotal[5] - chestCount[5]) * 100;

		std::string info =
			"<cj>1 Key Chests:</c> " + std::to_string(chestCount[0]) + " out of " + std::to_string(chestTotal[0]) +
			"\n<cg>5 Key Chests:</c> " + std::to_string(chestCount[1]) + " out of " + std::to_string(chestTotal[1]) +
			"\n<cy>10 Key Chests:</c> " + std::to_string(chestCount[2]) + " out of " + std::to_string(chestTotal[2]) +
			"\n<co>25 Key Chests:</c> " + std::to_string(chestCount[3]) + " out of " + std::to_string(chestTotal[3]) +
			"\n<cr>50 Key Chests:</c> " + std::to_string(chestCount[4]) + " out of " + std::to_string(chestTotal[4]) +
			"\n<cp>100 Key Chests:</c> " + std::to_string(chestCount[5]) + " out of " + std::to_string(chestTotal[5]) +
			"\n\nAmount of keys needed to full completion: " + std::to_string(keysNeeded);

		FLAlertLayer::create("Treasure Room Stats", info.c_str(), "OK")->show();
	}

public:
	static TreasureChecklistAlert *create()
	{
		auto ret = new TreasureChecklistAlert();
		if (ret && ret->init(420.f, 260.f))
		{
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

//	The Mod modifies the Garage Layer to add the new Button.
class $modify(ModLayer, GJGarageLayer){
	bool init(){
		if (!GJGarageLayer::init()) return false;

		NodeIDs::provideFor(this);

		auto menu = this->getChildByID("shards-menu");

		auto spr = CircleButtonSprite::createWithSpriteFrameName("chest_03_02_001.png", 1, CircleBaseColor::Gray, CircleBaseSize::Small);
		auto infoButton = CCMenuItemSpriteExtra::create(
			spr,
			this,
			menu_selector(ModLayer::onChestButton));

		menu->addChild(infoButton);
		menu->updateLayout();

		return true;
	}

	void onChestButton(CCObject *)
	{
		TreasureChecklistAlert::create()->show();
	}
}
;