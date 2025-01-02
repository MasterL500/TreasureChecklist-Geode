#include "SecretRewardsListAlert.hpp"

//  Structure for Icons
struct IconParameters : public CCObject
{
    UnlockType p_iconType;
    int p_iconID;
    int p_chestID;

    /// @brief
    /// @param icon
    /// @param id

    IconParameters(UnlockType type, int id, int chest) : p_iconType(type), p_iconID(id), p_chestID(chest)
    {
        this->autorelease();
    }
};

bool SecretRewardsListAlert::setup()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto showWraith = Mod::get()->getSettingValue<bool>("wraith-page");
    loadData();

    //  Background for the Icon Lists
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({winSize.width / 2.f, winSize.height / 2.f + 2.f});
    iconListBG->setColor({133, 68, 41});
    iconListBG->setContentSize({430, 176});
    this->addChild(iconListBG);

    //  Info Button (Menu)
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({405.f, 215.f});
    infoMenu->ignoreAnchorPointForPosition(false);
    infoMenu->setID("info-menu");
    this->addChild(infoMenu);

    //  Info Button (Itself)
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(SecretRewardsListAlert::onInfo));

    infoButton->setPosition(infoMenu->getContentSize());
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({winSize.width / 2, winSize.height / 2 - 104.0f});
    navMenu->setLayout(RowLayout::create()
                           ->setGap(6.f));
    navMenu->setID("navigation-menu");
    this->addChild(navMenu);

    //  Settings Button (Itself)
    auto settingsButton = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSpriteFrameName("geode.loader/settings.png", 1.25f),
        this,
        menu_selector(SecretRewardsListAlert::onSettings));

    settingsButton->setID("settings-button");
    navMenu->addChild(settingsButton);

    //  Navigation Menu (Buttons)
    for (int ii = 1; ii <= m_totalPages - !showWraith; ii++)
    {
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
        nullptr // menu_selector(SecretRewardsListAlert::onPageButton)
    );

    prevBtn->setID("arrow-prev-button");
    prevBtn->setTag(1);

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);

    auto nextBtn = CCMenuItemSpriteExtra::create(
        nextSpr,
        this,
        nullptr // menu_selector(SecretRewardsListAlert::onPageButton)
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
    
    onNavButton(navMenu->getChildByTag(1));

    this->m_noElasticity = true;
    this->setID("treasure-checklist-popup");
    return true;
};

void SecretRewardsListAlert::loadData()
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
                rewardObject->m_specialRewardItem == SpecialRewardItem::CustomItem && rewardObject->m_unlockType != (UnlockType)0)
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
        if (gsm->isItemUnlocked(icon.first, icon.second))
        {
            m_chestCount[7]++;
        }
        m_chestTotal[7]++;
    }
}

bool isMiscReward(UnlockType type)
{
    return
        type == UnlockType::Col1 ||
        type == UnlockType::Col2 ||
        type == UnlockType::Streak ||
        type == UnlockType::Death ||
        type == UnlockType::GJItem ||
        type == UnlockType::ShipFire;
}

void SecretRewardsListAlert::createIconPage(int ID, int index)
{
    auto menu = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);

    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto orderRewards = Mod::get()->getSettingValue<bool>("sorted-rewards");
    auto groupRewards = Mod::get()->getSettingValue<bool>("group-chests");

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

    std::vector<std::vector<std::pair<UnlockType, int>>> chestList;
    auto gsm = GameStatsManager::sharedState();

    for (auto chestEntry : CCDictionaryExt<intptr_t, GJRewardItem *>(gsm->m_allTreasureRoomChests))
    {
        if (chestEntry.first / 1000 != ID - 1)
        {
            continue;
        }

        std::vector<std::pair<UnlockType, int>> chest;
        auto rewardItem = chestEntry.second;
        for (auto rewardObject : CCArrayExt<GJRewardObject *>(rewardItem->m_rewardObjects))
        {
            if (
                rewardObject->m_specialRewardItem == SpecialRewardItem::CustomItem &&
                rewardObject->m_unlockType != UnlockType::GJItem &&
                rewardObject->m_unlockType != (UnlockType)0)
            {
                if (isMiscReward(rewardObject->m_unlockType) && !showMiscRewards)
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

    if (ID == 8)
    {
        for (auto const &icon : gsm->m_wraithIcons)
        {
            chestList.push_back({{icon.first, icon.second}});
        }
    }

    if (ID != 7 || !orderRewards)
    {
        auto multipleItems = std::any_of(chestList.begin(), chestList.end(), [](
            const std::vector<std::pair<UnlockType, int>> &c)
        {
            return c.size() > 1;
        });
        if (multipleItems)
        {
            for (auto &chest : chestList)
            {
                std::sort(chest.begin(), chest.end(), [](
                    const std::pair<UnlockType, int> &a, const std::pair<UnlockType, int> &b)
                {
                    auto aMisc = isMiscReward(a.first);
                    auto bMisc = isMiscReward(b.first);
                    return aMisc != bMisc ? aMisc < bMisc : a.first != b.first ? a.first < b.first : a.second < b.second;
                });
            }
        }

        std::sort(chestList.begin(), chestList.end(), [](
            const std::vector<std::pair<UnlockType, int>> &av, const std::vector<std::pair<UnlockType, int>> &bv)
        {
            auto &a = av[0];
            auto &b = bv[0];
            auto aMisc = isMiscReward(a.first);
            auto bMisc = isMiscReward(b.first);
            return aMisc != bMisc ? aMisc < bMisc : a.first != b.first ? a.first < b.first : a.second < b.second;
        });
    }

    switch (ID)
    {

    case 4:
        if (groupRewards && showMiscRewards)
        {
            iconMenu->setLayout(RowLayout::create()
                                    ->setGap(14.0f)
                                    ->setGrowCrossAxis(true)
                                    ->setCrossAxisOverflow(false)
                                    ->setCrossAxisLineAlignment(AxisAlignment::Even));
        }
        break;

    case 5:
        if (groupRewards)
        {
            iconMenu->setLayout(RowLayout::create()
                                    ->setGap(24.0f)
                                    ->setGrowCrossAxis(true)
                                    ->setCrossAxisOverflow(false)
                                    ->setCrossAxisAlignment(AxisAlignment::Center)
                                    ->setCrossAxisLineAlignment(AxisAlignment::Even));
            iconMenu->setContentSize({440.f, 220.f});
        }
        break;

    case 6:
        if (groupRewards)
        {
            iconMenu->setLayout(RowLayout::create()
                                    ->setGap(24.0f)
                                    ->setGrowCrossAxis(true)
                                    ->setCrossAxisOverflow(false)
                                    ->setCrossAxisAlignment(AxisAlignment::Center)
                                    ->setCrossAxisLineAlignment(AxisAlignment::Even));
            iconMenu->setContentSize({440.f, 220.f});
        }
        break;

    case 7:

        if (orderRewards)
        {
            iconMenu->setLayout(RowLayout::create()
                                    ->setGap(25.0f)
                                    ->setAutoScale(false)
                                    ->setGrowCrossAxis(true)
                                    ->setCrossAxisOverflow(false)
                                    ->setCrossAxisAlignment(AxisAlignment::Center)
                                    ->setCrossAxisLineAlignment(AxisAlignment::Even));
            iconMenu->setContentSize({480.f, 220.f});
        }
        break;
    }

    int itemIndex = 1;

    for (auto const &chest : chestList)
    {
        if (chest.size() == 1)
        {
            if (ID == 7 && orderRewards)
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
                createItemLabeled(iconMenu, chest[0], fmt::format("{}{}", itemIndex, label).c_str());
                itemIndex++;
            }
            else
            {
                createItem(iconMenu, chest[0].first, chest[0].second);
            }
        }
        else if (groupRewards)
        {
            createItemGroup(iconMenu, chest);
        }
    }
};

void SecretRewardsListAlert::createNavButton(CCMenu *menu, int tag, bool active)
{
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1)   ? "chest_03_02_001.png"
                   : (tag == 2) ? "chest_04_02_001.png"
                   : (tag == 3) ? "chest_05_02_001.png"
                   : (tag == 4) ? "chest_06_02_001.png"
                   : (tag == 5) ? "chest_07_02_001.png"
                   : (tag == 6) ? "chest_08_02_001.png"
                   : (tag == 7) ? "chest_09_02_001.png"
                                : "GJ_secretLock4_001.png";

    //  Base color based on whenever the current page is on
    auto baseColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
    auto buttonSpr = IconSelectButtonSprite::createWithSpriteFrameName(sprName, 1.25F, baseColor);

    if (tag == 8)
        buttonSpr = IconSelectButtonSprite::createWithSprite("TC_WraithIcon.png"_spr, 1.25F, baseColor);

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(SecretRewardsListAlert::onNavButton));

    //  Adds ID and Tag
    button->setID("chest-button-" + std::to_string(tag));
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void SecretRewardsListAlert::onNavButton(CCObject *sender)
{
    auto showWraith = Mod::get()->getSettingValue<bool>("wraith-page");
    auto menu = this->getChildByID("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();
    if (tag == 8 && !showWraith)
    {
        tag = 1;
        sender = navMenu->getChildByTag(1);
    }

    auto oldPage = m_page;
    m_page = tag;

    auto shopTitle = (tag == 1)   ? "Rewards (1 Key chests)"
                     : (tag == 2) ? "Rewards (5 Key chests)"
                     : (tag == 3) ? "Rewards (10 Key chests)"
                     : (tag == 4) ? "Rewards (25 Key chests)"
                     : (tag == 5) ? "Rewards (50 Key chests)"
                     : (tag == 6) ? "Rewards (100 Key chests)"
                     : (tag == 7) ? "Rewards (Golden Key chests)"
                                  : "Rewards (Wraith's Vault)";

    this->setTitle(shopTitle);

    if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(navMenu->getChildByTag(oldPage)))
    {
        static_cast<CCSprite*>(navButton->getNormalImage())->setDisplayFrame(
            CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
        navButton->updateSprite();
        navMenu->updateLayout();
    }

    if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
    {
        static_cast<CCSprite*>(navButton->getNormalImage())->setDisplayFrame(
            CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
        navButton->updateSprite();
        navMenu->updateLayout();
    }

    if (showWraith && !navMenu->getChildByTag(8))
    {
        createNavButton(navMenu, 8, false);
    }
    else if (!showWraith && navMenu->getChildByTag(8))
    {
        navMenu->removeChildByTag(8);
        navMenu->updateLayout();
    }

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
void SecretRewardsListAlert::onPageButton(CCObject * sender){
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

    createIconPage(m_page, tag);
};*/

void SecretRewardsListAlert::createItem(CCMenu *menu, UnlockType iconType, int iconID)
{
    auto checkmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
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
    iconButton->setUserObject(new IconParameters(iconType, iconID, menu->getTag()));

    menu->addChild(iconButton);
    menu->updateLayout();
}

void SecretRewardsListAlert::createItemLabeled(CCMenu *menu, std::pair<UnlockType, int> icon, const char *label)
{
    auto checkmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gsm = GameStatsManager::sharedState();
    UnlockType iconType{icon.first};

    auto baseSprite = CCSprite::create();
    baseSprite->setContentSize({ 40, 45 });

    auto iconSprite = GJItemIcon::createBrowserItem(
        iconType, icon.second);

    iconSprite->setPosition({baseSprite->getContentSize().width / 2, baseSprite->getContentSize().height});
    iconSprite->setAnchorPoint({0.5, 1});
    baseSprite->addChild(iconSprite);

    if (gsm->isItemUnlocked(iconType, icon.second) && checkmark)
    {
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSprite->getContentSize() / 2);
        checkmark->setScale(0.75f);

        iconSprite->darkenStoreItem({30, 30, 30});
        iconSprite->addChild(checkmark);
    }
    else
    {
        if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2)
        {
            auto colorType = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
            colorType->setPosition(iconSprite->getContentSize() / 2);
            colorType->setScale(0.5f);

            iconSprite->addChild(colorType);
        }

        auto text = CCLabelBMFont::create(label, "goldFont.fnt");
        text->setPosition({baseSprite->getContentSize().width / 2, 0.0f});
        text->setAnchorPoint({0.5, 0});
        text->setScale(0.5f);

        baseSprite->addChild(text);
    }

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        baseSprite,
        this,
        menu_selector(SecretRewardsListAlert::onIcon));

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(icon.first, icon.second, menu->getTag()));

    menu->addChild(iconButton);
    menu->updateLayout();
}

void SecretRewardsListAlert::createItemGroup(CCMenu *menu, std::vector<std::pair<UnlockType, int>> icons)
{
    //  log::info("Function called, map size {}", icons.size());

    auto auxMenu = CCMenu::create();
    auxMenu->setContentSize({80.0, 40.0});
    auxMenu->setLayout(RowLayout::create()
                           ->setGap(12.0f)
                           ->setGrowCrossAxis(true)
                           ->setCrossAxisLineAlignment(AxisAlignment::Even));

    if (icons.size() == 3)
    {
        auxMenu->setContentSize({120.f, 40.f});
    }
    else if (icons.size() == 4)
    {
        auxMenu->setContentSize({160.f, 40.f});
    }

    for (auto const &[iconType, iconID] : icons)
    {
        auto checkmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
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
        iconButton->setUserObject(new IconParameters(iconType, iconID, menu->getTag()));

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

    menu->addChild(auxMenu);
    menu->updateLayout();
}

//	When a specific Icon Button is pressed
void SecretRewardsListAlert::onIcon(CCObject *sender)
{
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());

    ItemInfoPopup::create(parameters->p_iconID, parameters->p_iconType)->show();
};

//  Shows the stats of the Player
void SecretRewardsListAlert::onInfo(CCObject *sender)
{
    std::string info =
        "<cj>1-Key:</c> " + std::to_string(m_chestCount[0]) + " out of " + std::to_string(m_chestTotal[0]) +
        "\n<cj>5-Keys:</c> " + std::to_string(m_chestCount[1]) + " out of " + std::to_string(m_chestTotal[1]) +
        "\n<cj>10-Keys:</c> " + std::to_string(m_chestCount[2]) + " out of " + std::to_string(m_chestTotal[2]) +
        "\n<cg>25-Keys:</c> " + std::to_string(m_chestCount[3]) + " out of " + std::to_string(m_chestTotal[3]) +
        "\n<cg>50-Keys:</c> " + std::to_string(m_chestCount[4]) + " out of " + std::to_string(m_chestTotal[4]) +
        "\n<cg>100-Keys:</c> " + std::to_string(m_chestCount[5]) + " out of " + std::to_string(m_chestTotal[5]) +
        "\n<cy>Gold Keys:</c> " + std::to_string(m_chestCount[6]) + " out of " + std::to_string(m_chestTotal[6]);

    if (Mod::get()->getSettingValue<bool>("wraith-page"))
    {
        info += "\n<cp>Wraith's:</c> " + std::to_string(m_chestCount[7]) + " out of " + std::to_string(m_chestTotal[7]);
    }

    FLAlertLayer::create("Unlocked Items", info.c_str(), "OK")->show();
};

//  Opens the settings of the mod
void SecretRewardsListAlert::onSettings(CCObject *sender)
{
    geode::openSettingsPopup(Mod::get());
}

//  Create
SecretRewardsListAlert *SecretRewardsListAlert::create()
{
    auto ret = new SecretRewardsListAlert();

    if (ret->initAnchored(450.f, 260.f))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};