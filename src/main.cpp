#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "SecretRewardsListAlert.hpp"

using namespace geode::prelude;

//	Garage Layer (Icon Kit)
class $modify(GarageLayer, GJGarageLayer)
{
	bool init()
	{
		if (!GJGarageLayer::init())
			return false;

		if (Mod::get()->getSettingValue<bool>("garage-button"))
		{
			auto menu = this->getChildByID("shards-menu");

			auto spr = CircleButtonSprite::createWithSpriteFrameName("chest_03_02_001.png", 1, CircleBaseColor::Gray, CircleBaseSize::Small);
			auto button = CCMenuItemSpriteExtra::create(
				spr,
				this,
				menu_selector(GarageLayer::onChestButton));
			button->setID("Treasure-Checklist-Button");

			menu->addChild(button);
			menu->updateLayout();
		}

		return true;
	}

	void onChestButton(CCObject *)
	{
		SecretRewardsListAlert::create()->show();
	}
};

// Treasure Room Layer
class $modify(TreasureRoomLayer, SecretRewardsLayer)
{
	bool init(bool p0)
	{
		if (!SecretRewardsLayer::init(p0))
			return false;

		if (Mod::get()->getSettingValue<bool>("room-button"))
		{
			auto menu = CCMenu::create();
			menu->setPosition({30.0f, 30.0f});
			menu->setID("Treasure-Checklist-Menu");
			menu->setZOrder(5);

			auto button = CCMenuItemSpriteExtra::create(
				CCSpriteGrayscale::createWithSpriteFrameName("GJ_infoIcon_001.png"),
				this,
				menu_selector(TreasureRoomLayer::onChestButton));
			button->setID("Treasure-Checklist-Button");

			menu->addChild(button);
			menu->updateLayout();

			this->addChild(menu);
		}

		return true;
	}

	void onChestButton(CCObject *)
	{
		SecretRewardsListAlert::create()->show();
	}
};

$execute
{
	listenForAllSettingChanges([](const std::string_view key, std::shared_ptr<SettingV3> setting)
							   {
			if (auto rewardsList = static_cast<SecretRewardsListAlert *>(CCScene::get()->getChildByIDRecursive("Treasure-Checklist-Popup"))){
				rewardsList->onNavButton(rewardsList->getChildByIDRecursive("navigation-menu")->getChildByTag(rewardsList->m_page));
			} });
};