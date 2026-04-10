#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class SecretRewardsListAlert : public Popup
{
protected:
	std::array<int, 8> m_chestCount = {0, 0, 0, 0, 0, 0, 0, 0};
	std::array<int, 8> m_chestTotal = {0, 0, 0, 0, 0, 0, 0, 0};
	unsigned int m_totalPages = 8;

	CCMenu *m_iconMenu = nullptr;
	CCMenu *m_pageMenu = nullptr;
	CCMenu *m_navMenu = nullptr;

	CCMenuItemSpriteExtra *m_prevBtn = nullptr;
	CCMenuItemSpriteExtra *m_nextBtn = nullptr;

	InfoAlertButton *m_infoButton = nullptr;

	gd::set<std::pair<UnlockType, int>> m_extraWraithIcons = {};

	bool init();
	void createItem(std::pair<UnlockType, int> icon);
	void createItemGroup(std::vector<std::pair<UnlockType, int>> icons);
	void createItemLabeled(std::pair<UnlockType, int> icon, const char *text, bool isCode = false);

	void createNavButton(int tag, bool isActive);
	void createIconPage(int id, int page);

	void onPageButton(CCObject *);
	void onSettings(CCObject *);
	void onInfo(CCObject *);

	void loadStats();
	void loadExtraIcons();

	//  Online Features (For Wraith codes)
	TaskHolder<web::WebResponse> m_listener;
	void fetchWraithData();

	std::vector<matjson::Value> m_fetchedData;
	gd::string getWraithCode(UnlockType type, int id);

public:
	unsigned int m_page = 0;
	unsigned int m_subPage = 0;

	static SecretRewardsListAlert *create();
	void onNavButton(CCObject *sender);
	void onIcon(CCObject *sender);
};

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