#include "SecretRewardsListAlert.hpp"

bool SecretRewardsListAlert::init()
{
	if (!Popup::init(450.f, 260.f))
		return false;

	fetchWraithData();
	loadStats();

	//  Background
	auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
	background->setContentSize({430, 180});
	background->setColor({133, 68, 41});
	m_mainLayer->addChildAtPosition(background, Anchor::Center, ccp(0, 2), false);

	//  Info Stats
	std::string info =
		"<cj>1-Key:</c> " + std::to_string(m_chestCount[0]) + " out of " + std::to_string(m_chestTotal[0]) +
		"\n<cj>5-Keys:</c> " + std::to_string(m_chestCount[1]) + " out of " + std::to_string(m_chestTotal[1]) +
		"\n<cj>10-Keys:</c> " + std::to_string(m_chestCount[2]) + " out of " + std::to_string(m_chestTotal[2]) +
		"\n<cg>25-Keys:</c> " + std::to_string(m_chestCount[3]) + " out of " + std::to_string(m_chestTotal[3]) +
		"\n<cg>50-Keys:</c> " + std::to_string(m_chestCount[4]) + " out of " + std::to_string(m_chestTotal[4]) +
		"\n<cg>100-Keys:</c> " + std::to_string(m_chestCount[5]) + " out of " + std::to_string(m_chestTotal[5]) +
		"\n<cs>Gold Keys:</c> " + std::to_string(m_chestCount[6]) + " out of " + std::to_string(m_chestTotal[6]);

	if (Mod::get()->getSettingValue<bool>("wraith-page"))
	{
		info += "\n<ca>Wraith's:</c> " + std::to_string(m_chestCount[7]) + " out of " + std::to_string(m_chestTotal[7]);
	}

	//  Info Button
	auto infoButton = InfoAlertButton::create("Unlocked Items", info.c_str(), 1);
	infoButton->setID("stats-button");
	m_buttonMenu->addChildAtPosition(infoButton, Anchor::TopRight, ccp(-20, -20), false);

	//  Navigation Menu
	m_navMenu = CCMenu::create();
	m_navMenu->setID("navigation-menu");
	m_navMenu->setLayout(RowLayout::create()
							 ->setGap(6.f));
	m_mainLayer->addChildAtPosition(m_navMenu, Anchor::Bottom, ccp(0, 24), false);

	//  Settings Button
	auto settingsButton = CCMenuItemSpriteExtra::create(
		IconSelectButtonSprite::createWithSprite("SettingsIcon.png"_spr, 1.5F),
		this,
		menu_selector(SecretRewardsListAlert::onSettings));
	settingsButton->setID("settings-button");
	m_navMenu->addChild(settingsButton);
	m_navMenu->updateLayout();

	//  Navigation Menu Buttons
	for (int ii = 0; ii < m_totalPages; ii++)
		createNavButton(ii, ii == 0);

	//  Page Navigation Menu (within the Icon Menu)
	m_pageMenu = CCMenu::create();
	m_pageMenu->setContentSize(m_mainLayer->getContentSize());
	m_pageMenu->setID("page-menu");
	m_mainLayer->addChildAtPosition(m_pageMenu, Anchor::BottomLeft, ccp(0, 0), false);

	//  Page Navigation Buttons
	m_prevBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(SecretRewardsListAlert::onPageButton));
	m_prevBtn->setID("prev-page-button");
	m_prevBtn->setVisible(false);
	m_prevBtn->setTag(0);

	auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	nextSpr->setFlipX(true);

	m_nextBtn = CCMenuItemSpriteExtra::create(
		nextSpr,
		this,
		menu_selector(SecretRewardsListAlert::onPageButton));
	m_nextBtn->setID("next-page-button");
	m_nextBtn->setTag(1);

	m_pageMenu->addChildAtPosition(m_prevBtn, Anchor::Left, ccp(-20, 0), false);
	m_pageMenu->addChildAtPosition(m_nextBtn, Anchor::Right, ccp(20, 0), false);

	//  Creates the Menu for the Icons
	m_iconMenu = CCMenu::create();
	m_iconMenu->setContentSize({530.0f, 220.0f});
	m_iconMenu->setLayout(RowLayout::create()
							  ->setGap(14.0f)
							  ->setAutoScale(false)
							  ->setGrowCrossAxis(true)
							  ->setCrossAxisOverflow(false)
							  ->setCrossAxisLineAlignment(AxisAlignment::Even));
	m_iconMenu->setID("icon-menu");
	m_iconMenu->setScale(0.8f);
	m_mainLayer->addChildAtPosition(m_iconMenu, Anchor::Center, ccp(0, 2), false);

	onNavButton(m_navMenu->getChildByTag(0));

	this->setID("treasure-checklist-popup");
	return true;
}

//  Fetches the data from Wraith icons in a separate repository
void SecretRewardsListAlert::fetchWraithData()
{
	auto req = web::WebRequest();

	m_listener.spawn(
		req.get("https://raw.githubusercontent.com/MasterL500/GD-Mods-Data/main/Wraith.json"),
		[this](web::WebResponse res)
		{
			if (res.ok() && res.json().isOk())
			{
				auto rawData = res.json().unwrapOrDefault();
				m_fetchedData = rawData.as<std::vector<matjson::Value>>().unwrap();
				log::debug("Data Loaded");
			}
			else
			{
				Notification::create("There was an error fetching Wraith Codes", NotificationIcon::Error)->show();
				log::error("Failed on loading data");
			}
		});
}

//  Returns the code of the current wrait icon (if it exists)
gd::string SecretRewardsListAlert::getWraithCode(UnlockType type, int id)
{
	for (auto &data : m_fetchedData)
	{
		auto iconType = static_cast<UnlockType>(data["iconType"].asInt().unwrapOr(0));
		auto iconID = data["iconID"].asInt().unwrapOr(0);
		auto iconCode = data["code"].asString().unwrapOr("");

		if (type == iconType && id == iconID && !utils::string::equalsIgnoreCase(iconCode, ""))
		{
			return iconCode;
		}
	}

	return "????";
}

//  Loads the data of the currently opened chests
void SecretRewardsListAlert::loadStats()
{
	auto gsm = GameStatsManager::sharedState();

	for (auto chestEntry : CCDictionaryExt<intptr_t, GJRewardItem *>(gsm->m_allTreasureRoomChests))
	{
		auto chestID = chestEntry.first / 1000;
		if (chestID > 6)
		{
			continue;
		}

		for (auto rewardObject : CCArrayExt<GJRewardObject *>(chestEntry.second->m_rewardObjects))
		{
			if (
				rewardObject->m_specialRewardItem == SpecialRewardItem::CustomItem && rewardObject->m_unlockType != UnlockType(0))
			{
				if (gsm->isItemUnlocked(rewardObject->m_unlockType, rewardObject->m_itemID))
				{
					m_chestCount[chestID]++;
				}
				m_chestTotal[chestID]++;
			}
		}
	}

	for (auto const &icon : gsm->m_wraithIcons)
	{
		//  Duplicate
		if (icon.first == UnlockType::Swing && icon.second == 68)
			continue;

		//  Duplicate
		if (icon.first == UnlockType::Ball && icon.second == 50)
			continue;

		if (gsm->isItemUnlocked(icon.first, icon.second))
		{
			m_chestCount[7]++;
		}
		m_chestTotal[7]++;
	}

	//  Adds the icons without Wraith info into the count
	//  Random Gauntlet announcement Cube
	if (gsm->isItemUnlocked(UnlockType::Cube, 351))
		m_chestCount[7]++;

	//  Funhouse Cube
	if (gsm->isItemUnlocked(UnlockType::Cube, 321))
		m_chestCount[7]++;

	//  Space Invaders UFO
	if (gsm->isItemUnlocked(UnlockType::Bird, 57))
		m_chestCount[7]++;

	//  GD Gangster Rap UFO
	if (gsm->isItemUnlocked(UnlockType::Bird, 71))
		m_chestCount[7]++;

	//  Adds the extra icons
	m_chestTotal[7] += 4;
}

//  Returns if the unlock is a non-icon
bool isExtraTypeSecret(UnlockType type)
{
	return type == UnlockType::Col1 ||
		   type == UnlockType::Col2 ||
		   type == UnlockType::Streak ||
		   type == UnlockType::Death ||
		   type == UnlockType::GJItem ||
		   type == UnlockType::ShipFire;
}

//  Creates a page of the Icons
void SecretRewardsListAlert::createIconPage(int ID, int page)
{
	//  Settings Flags
	auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
	auto orderRewards = Mod::get()->getSettingValue<bool>("sorted-rewards");
	auto groupRewards = Mod::get()->getSettingValue<bool>("group-chests");
	auto snitchCodes = Mod::get()->getSettingValue<bool>("wraith-codes");

	//  Removes all current icons of the menu and updates the tag
	m_iconMenu->removeAllChildren();
	m_iconMenu->updateLayout();
	m_iconMenu->setTag(ID);

	//  Default content size and layout format
	m_iconMenu->setContentSize({530.0f, 220.0f});
	m_iconMenu->setLayout(RowLayout::create()
							  ->setGap(14.0f)
							  ->setAutoScale(false)
							  ->setGrowCrossAxis(true)
							  ->setCrossAxisOverflow(false)
							  ->setCrossAxisLineAlignment(AxisAlignment::Even));

	//  To fetch the icons data
	std::vector<std::vector<std::pair<UnlockType, int>>> chestList;
	auto gsm = GameStatsManager::sharedState();

	for (auto chestEntry : CCDictionaryExt<intptr_t, GJRewardItem *>(gsm->m_allTreasureRoomChests))
	{
		if (chestEntry.first / 1000 != ID)
		{
			continue;
		}

		std::vector<std::pair<UnlockType, int>> chest;
		for (auto rewardObject : CCArrayExt<GJRewardObject *>(chestEntry.second->m_rewardObjects))
		{
			if (
				rewardObject->m_specialRewardItem == SpecialRewardItem::CustomItem &&
				rewardObject->m_unlockType != UnlockType::GJItem &&
				rewardObject->m_unlockType != UnlockType(0))
			{
				if (isExtraTypeSecret(rewardObject->m_unlockType) && !showMiscRewards)
				{
					continue;
				}
				chest.push_back({rewardObject->m_unlockType, rewardObject->m_itemID});
			}
		}

		if (!chest.empty())
		{
			if (groupRewards)
			{
				chestList.push_back(chest);
			}
			else
			{
				for (auto const &icon : chest)
				{
					chestList.push_back({icon});
				}
			}
		}
	}

	if (ID == 7)
	{
		for (auto const &icon : gsm->m_wraithIcons)
		{
			chestList.push_back({{icon.first, icon.second}});
		}
	}

	if (ID != 6 || !orderRewards)
	{
		auto multipleItems = std::any_of(chestList.begin(), chestList.end(), [](const std::vector<std::pair<UnlockType, int>> &c)
										 { return c.size() > 1; });
		if (multipleItems)
		{
			for (auto &chest : chestList)
			{
				std::sort(chest.begin(), chest.end(), [](const std::pair<UnlockType, int> &a, const std::pair<UnlockType, int> &b)
						  {
					auto aMisc = isExtraTypeSecret(a.first);
					auto bMisc = isExtraTypeSecret(b.first);
					return aMisc != bMisc ? aMisc < bMisc : a.first != b.first ? a.first < b.first : a.second < b.second; });
			}
		}

		std::sort(chestList.begin(), chestList.end(), [](const std::vector<std::pair<UnlockType, int>> &av, const std::vector<std::pair<UnlockType, int>> &bv)
				  {
			auto &a = av[0];
			auto &b = bv[0];
			auto aMisc = isExtraTypeSecret(a.first);
			auto bMisc = isExtraTypeSecret(b.first);
			return aMisc != bMisc ? aMisc < bMisc : a.first != b.first ? a.first < b.first : a.second < b.second; });
	}

	//  Makes the format of the menu, based on the page
	switch (ID)
	{
	case 3:
		if (groupRewards && showMiscRewards)
		{
			m_iconMenu->setLayout(RowLayout::create()
									  ->setGap(14.0f)
									  ->setGrowCrossAxis(true)
									  ->setCrossAxisOverflow(false)
									  ->setCrossAxisLineAlignment(AxisAlignment::Even));
		}
		break;

	case 4:
		if (groupRewards)
		{
			m_iconMenu->setLayout(RowLayout::create()
									  ->setGap(24.0f)
									  ->setGrowCrossAxis(true)
									  ->setCrossAxisOverflow(false)
									  ->setCrossAxisAlignment(AxisAlignment::Center)
									  ->setCrossAxisLineAlignment(AxisAlignment::Even));
			m_iconMenu->setContentSize({440.f, 220.f});
		}
		break;

	case 5:
		if (groupRewards)
		{
			m_iconMenu->setLayout(RowLayout::create()
									  ->setGap(24.0f)
									  ->setGrowCrossAxis(true)
									  ->setCrossAxisOverflow(false)
									  ->setCrossAxisAlignment(AxisAlignment::Center)
									  ->setCrossAxisLineAlignment(AxisAlignment::Even));
			m_iconMenu->setContentSize({440.f, 220.f});
		}
		break;

	case 6:
		if (orderRewards)
		{
			m_iconMenu->setLayout(RowLayout::create()
									  ->setGap(10.0f)
									  ->setAutoScale(false)
									  ->setGrowCrossAxis(true)
									  ->setCrossAxisOverflow(false)
									  ->setCrossAxisAlignment(AxisAlignment::Between)
									  ->setCrossAxisLineAlignment(AxisAlignment::Even));
			m_iconMenu->setContentSize({480.f, 210.f});
		}
		break;

	case 7:
		if (snitchCodes)
		{
			m_iconMenu->setLayout(RowLayout::create()
									  ->setGap(10.0f)
									  ->setAutoScale(false)
									  ->setGrowCrossAxis(true)
									  ->setCrossAxisOverflow(false)
									  ->setAxisAlignment(AxisAlignment::Even)
									  ->setCrossAxisAlignment(AxisAlignment::Between)
									  ->setCrossAxisLineAlignment(AxisAlignment::Between));
			m_iconMenu->setContentSize({520.f, 200.f});
		}
	}

	int itemIndex = 1;
	int maxToDivide = 18;

	for (auto const &chest : chestList)
	{
		if (chest.size() == 1)
		{
			if (ID == 6 && orderRewards)
			{
				int twoDigits = itemIndex % 100;
				int lastDigit = twoDigits % 10;
				auto label = "th";
				if ((lastDigit == 1 || lastDigit == 2 || lastDigit == 3) && (twoDigits < 10 || twoDigits > 20))
				{
					if (lastDigit == 1)
					{
						label = "st";
					}
					else if (lastDigit == 2)
					{
						label = "nd";
					}
					else if (lastDigit == 3)
					{
						label = "rd";
					}
				}
				createItemLabeled(chest[0], fmt::format("{}{}", itemIndex, label).c_str());
				itemIndex++;
			}
			else
			{
				if (ID == 7 && !snitchCodes)
				{
					//  I love it when Robtop adds icons without wraith information, so I gotta manually add them here.

					//  Funhouse Cube
					if (chest[0].first == UnlockType::Cube && chest[0].second == 343)
						createItem({UnlockType::Cube, 321});

					//  Random Gauntlet announcement Cube
					if (chest[0].first == UnlockType::Cube && chest[0].second == 390)
						createItem({UnlockType::Cube, 351});
					//  Space Invaders UFO
					if (chest[0].first == UnlockType::Bird && chest[0].second == 63)
						createItem({UnlockType::Bird, 57});

					//  GD Gangster Rap UFO
					if (chest[0].first == UnlockType::Bird && chest[0].second == 116)
						createItem({UnlockType::Bird, 71});

					//  Skips this icon because it's a duplicate
					if (chest[0].first == UnlockType::Swing && chest[0].second == 68)
						continue;

					//  Skips this icon because it's a duplicate
					if (chest[0].first == UnlockType::Ball && chest[0].second == 50)
						continue;
				}
				else if (ID == 7 && snitchCodes)
				{
					if (m_subPage == 0 && itemIndex > maxToDivide)
					{
						itemIndex++;
						continue;
					}

					if (m_subPage == 1 && itemIndex <= maxToDivide)
					{
						itemIndex++;
						continue;
					}

					//  Funhouse Cube
					if (chest[0].first == UnlockType::Cube && chest[0].second == 343)
					{
						createItemLabeled({UnlockType::Cube, 321}, getWraithCode(UnlockType::Cube, 321).c_str(), true);
						itemIndex++;
					}

					//  Random Gauntlet announcement Cube
					if (chest[0].first == UnlockType::Cube && chest[0].second == 390)
					{
						createItemLabeled({UnlockType::Cube, 351}, getWraithCode(UnlockType::Cube, 351).c_str(), true);
						itemIndex++;
					}

					//  Space Invaders UFO
					if (chest[0].first == UnlockType::Bird && chest[0].second == 63)
					{
						createItemLabeled({UnlockType::Bird, 57}, getWraithCode(UnlockType::Bird, 57).c_str(), true);
						itemIndex++;
					}

					//  GD Gangster Rap UFO
					if (chest[0].first == UnlockType::Bird && chest[0].second == 116)
					{
						createItemLabeled({UnlockType::Bird, 71}, getWraithCode(UnlockType::Bird, 71).c_str(), true);
						itemIndex++;
					}

					//  Skips this icon because it's a duplicate
					if (chest[0].first == UnlockType::Swing && chest[0].second == 68)
						continue;

					//  Skips this icon because it's a duplicate
					if (chest[0].first == UnlockType::Ball && chest[0].second == 50)
						continue;

					createItemLabeled(chest[0], getWraithCode(chest[0].first, chest[0].second).c_str(), true);
					itemIndex++;
					continue;
				}

				createItem(chest[0]);
			}
		}
		else if (groupRewards)
		{
			createItemGroup(chest);
		}
	}
}

//  Creates a button for the Navigation menu
void SecretRewardsListAlert::createNavButton(int tag, bool active)
{
	//  Sprite List
	std::vector<const char *> sprites = {
		"chest_03_02_001.png",
		"chest_04_02_001.png",
		"chest_05_02_001.png",
		"chest_06_02_001.png",
		"chest_07_02_001.png",
		"chest_08_02_001.png",
		"chest_09_02_001.png",
		"GJ_secretLock4_001.png"};

	//  Base color based on whenever the current page is on
	auto buttonColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
	auto buttonSpr = IconSelectButtonSprite::createWithSpriteFrameName(sprites[tag], 1.25f, buttonColor);

	//  If Wraith page
	if (tag == 7)
		buttonSpr = IconSelectButtonSprite::createWithSprite("WraithIcon.png"_spr, 1.25F, buttonColor);

	//  Button
	auto button = CCMenuItemSpriteExtra::create(
		buttonSpr,
		this,
		menu_selector(SecretRewardsListAlert::onNavButton));
	button->setID(fmt::format("navigation-button-{:02}", tag + 1));
	button->setTag(tag);

	//  Adds button to menu and updates layout.
	m_navMenu->addChild(button);
	m_navMenu->updateLayout();
}

//  When a Navigation Button is pressed
void SecretRewardsListAlert::onNavButton(CCObject *sender)
{
	//  Notification::create(fmt::format("Button {} Pressed", sender->getTag() + 1), NotificationIcon::Info, 0.5f)->show();
	auto showWraith = Mod::get()->getSettingValue<bool>("wraith-page");
	auto tag = sender->getTag();

	if (tag == 7 && !showWraith)
	{
		sender = m_navMenu->getChildByTag(0);
		tag = 0;
	}

	m_pageMenu->setVisible(tag == 7);

	auto oldPage = m_page;
	m_page = tag;

	//  Sprite List
	std::vector<const char *> pageTitles = {
		"Rewards (1 Key chests)",
		"Rewards (5 Key chests)",
		"Rewards (10 Key chests)",
		"Rewards (25 Key chests)",
		"Rewards (50 Key chests)",
		"Rewards (100 Key chests)",
		"Rewards (Golden Chests)",
		"Rewards (Wraith's Vault)"};
	this->setTitle(pageTitles[tag]);

	//  Updates the sprite of the button of the previous page
	if (auto oldNavButton = static_cast<CCMenuItemSpriteExtra *>(m_navMenu->getChildByTag(oldPage)))
	{
		static_cast<CCSprite *>(oldNavButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
		oldNavButton->updateSprite();
		m_navMenu->updateLayout();
	}

	//  Updates the sprite of the button for the current page
	if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
	{
		static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
		navButton->updateSprite();
		m_navMenu->updateLayout();
	}

	//  If the Wraith Button is meant to be hidden, removes it from the Nav. Menu
	if (!showWraith && m_navMenu->getChildByTag(7))
	{
		m_navMenu->removeChildByTag(7);
		m_navMenu->updateLayout();
	}
	else if (!showWraith && !m_navMenu->getChildByTag(7))
	{
		createNavButton(7, false);
	}

	createIconPage(tag, 0);
}

//  Creates an Item
void SecretRewardsListAlert::createItem(std::pair<UnlockType, int> icon)
{
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto gsm = GameStatsManager::sharedState();

	auto iconSpr = GJItemIcon::createBrowserItem(
		icon.first,
		icon.second);

	if (gsm->isItemUnlocked(icon.first, icon.second) && checkmark)
	{
		auto checkmarkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
		checkmarkSpr->setPosition(iconSpr->getContentSize() / 2);
		checkmarkSpr->setScale(0.75f);

		iconSpr->darkenStoreItem({30, 30, 30});
		iconSpr->addChild(checkmarkSpr);
	}
	else
	{
		if (icon.first == UnlockType::Col1 || icon.first == UnlockType::Col2)
		{
			auto colorNum = (icon.first == UnlockType::Col1) ? "1" : "2";
			auto colorLabel = CCLabelBMFont::create(colorNum, "bigFont.fnt");
			colorLabel->setPosition(iconSpr->getContentSize() / 2);
			colorLabel->setScale(0.5f);

			iconSpr->addChild(colorLabel);
		}
	}

	//  Creates the Icon as a button itself.
	auto iconButton = CCMenuItemSpriteExtra::create(
		iconSpr,
		this,
		menu_selector(SecretRewardsListAlert::onIcon));

	//	Passes the user-defined IconParameters for its callback
	iconButton->setUserObject("icon-parameters"_spr, new IconParameters(icon.first, icon.second, m_iconMenu->getTag()));
	m_iconMenu->addChild(iconButton);
	m_iconMenu->updateLayout();
}

//  Creates an Item with a Label
void SecretRewardsListAlert::createItemLabeled(std::pair<UnlockType, int> icon, const char *text, bool isCode)
{
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto gsm = GameStatsManager::sharedState();

	auto baseSprite = CCSprite::create();
	baseSprite->setContentSize({50, 40});

	auto iconSpr = GJItemIcon::createBrowserItem(
		icon.first,
		icon.second);

	// iconSprite->setPosition({baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height});
	// iconSprite->setAnchorPoint({0.5, 0.5});
	// baseSprite->addChild(iconSprite);
	baseSprite->addChildAtPosition(iconSpr, Anchor::Center, ccp(0, 0), false);

	if (gsm->isItemUnlocked(icon.first, icon.second) && checkmark)
	{
		auto checkmarkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
		checkmarkSpr->setPosition(iconSpr->getContentSize() / 2);
		checkmarkSpr->setScale(0.75f);

		iconSpr->darkenStoreItem({30, 30, 30});
		iconSpr->addChild(checkmarkSpr);
	}
	else
	{
		if (icon.first == UnlockType::Col1 || icon.first == UnlockType::Col2)
		{
			auto colorNum = (icon.first == UnlockType::Col1) ? "1" : "2";
			auto colorLabel = CCLabelBMFont::create(colorNum, "bigFont.fnt");
			colorLabel->setPosition(iconSpr->getContentSize() / 2);
			colorLabel->setScale(0.5f);

			iconSpr->addChild(colorLabel);
		}

		auto label = TextArea::create(text, "goldFont.fnt", 1.f, 200, {0.5, 1}, 20.f, false);
		label->setScale(0.5f);

		if (isCode)
			label->setScale(0.4f);

		baseSprite->addChildAtPosition(label, Anchor::Bottom, ccp(0, 0), false);
	}

	//  Creates the Icon as a button itself.
	auto iconButton = CCMenuItemSpriteExtra::create(
		baseSprite,
		this,
		menu_selector(SecretRewardsListAlert::onIcon));
	iconButton->setContentSize({50, 60});
	baseSprite->setAnchorPoint({0.5, 1});
	baseSprite->setPositionY(60);

	if (isCode)
	{
		iconButton->setContentSize({80, 60});
		baseSprite->setPosition({40, 60});
	}

	//	Passes the user-defined IconParameters for its callback
	iconButton->setUserObject("icon-parameters"_spr, new IconParameters(icon.first, icon.second, m_iconMenu->getTag()));

	m_iconMenu->addChild(iconButton);
	m_iconMenu->updateLayout();
}

//  Creates an Item Group
void SecretRewardsListAlert::createItemGroup(std::vector<std::pair<UnlockType, int>> icons)
{
	auto auxMenu = CCMenu::create();
	auxMenu->setContentSize({80.0, 40.0});
	auxMenu->setLayout(RowLayout::create()
						   ->setGap(12.0f)
						   ->setGrowCrossAxis(true)
						   ->setCrossAxisLineAlignment(AxisAlignment::Even));

	if (icons.size() == 3)
	{
		auxMenu->setContentWidth(120.f);
	}
	else if (icons.size() == 4)
	{
		auxMenu->setContentWidth(160.f);
	}

	for (auto const &[iconType, iconID] : icons)
	{
		auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
		auto gsm = GameStatsManager::sharedState();

		auto iconSpr = GJItemIcon::createBrowserItem(
			iconType,
			iconID);

		if (gsm->isItemUnlocked(iconType, iconID) && checkmark)
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
			menu_selector(SecretRewardsListAlert::onIcon));

		//	Passes the user-defined IconParameters for its callback
		iconButton->setUserObject("icon-parameters"_spr, new IconParameters(iconType, iconID, m_iconMenu->getTag()));

		auxMenu->addChild(iconButton);
		auxMenu->updateLayout();
	}

	auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
	iconListBG->setColor({112, 51, 31});
	iconListBG->setContentSize({(auxMenu->getContentSize().width * 2) + 8.f, (auxMenu->getContentSize().height * 2) + 8.f});
	iconListBG->setPosition(auxMenu->getContentSize() / 2);
	iconListBG->setScale(0.5f);
	iconListBG->setZOrder(-1);
	auxMenu->addChild(iconListBG);

	m_iconMenu->addChild(auxMenu);
	m_iconMenu->updateLayout();
}

//  When the Arrow buttons are pressed
void SecretRewardsListAlert::onPageButton(CCObject *sender)
{
	auto tag = sender->getTag();
	m_subPage = tag;

	m_prevBtn->setVisible(tag == 1);
	m_nextBtn->setVisible(tag == 0);

	m_iconMenu->removeAllChildren();
	m_iconMenu->updateLayout();

	createIconPage(m_page, m_subPage);
};

//	When a specific Icon Button is pressed
void SecretRewardsListAlert::onIcon(CCObject *sender)
{
	auto parameters = static_cast<IconParameters *>(
		static_cast<CCNode *>(sender)->getUserObject("icon-parameters"_spr));

	ItemInfoPopup::create(parameters->p_iconID, parameters->p_iconType)->show();
};

//  Opens the settings of the Mod
void SecretRewardsListAlert::onSettings(CCObject *)
{
	geode::openSettingsPopup(Mod::get());
}

//  Creating the Popup
SecretRewardsListAlert *SecretRewardsListAlert::create()
{
	auto ret = new SecretRewardsListAlert;

	if (ret->init())
	{
		ret->autorelease();
		return ret;
	};

	delete ret;
	return nullptr;
}