#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/SecretRewardsLayer.hpp>
#include "TreasureCheckAlert.hpp"

using namespace geode::prelude;

//	The Mod modifies the Garage Layer to add the new Button.
class $modify(ModLayer, GJGarageLayer){
	bool init(){
		if (!GJGarageLayer::init()) return false;

		auto removedButton = Mod::get()->getSettingValue<bool>("remove-button");

		if(!removedButton){
			NodeIDs::provideFor(this);

			auto menu = this->getChildByID("shards-menu");

			auto spr = CircleButtonSprite::createWithSpriteFrameName("chest_03_02_001.png", 1, CircleBaseColor::Gray, CircleBaseSize::Small);
			auto button = CCMenuItemSpriteExtra::create(
				spr,
				this,
				menu_selector(ModLayer::onChestButton));
			button->setID("Treasure-Checklist-Button");
			
			menu->addChild(button);
			menu->updateLayout();
		}

		return true;
	}

	void onChestButton(CCObject *){
		TreasureCheckAlert::create()->show();
	}
};

class $modify(TreasureRoomLayer, SecretRewardsLayer){
	bool init(bool p0){
		if(!SecretRewardsLayer::init(p0)) return false;
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto menu = CCMenu::create();
		NodeIDs::provideFor(this);

		menu->setID("Treasure-Checklist-Menu");
		
		auto spr = CircleButtonSprite::createWithSpriteFrameName("chest_03_02_001.png", 1, CircleBaseColor::Gray, CircleBaseSize::Small);
		auto button = CCMenuItemSpriteExtra::create(
			spr,
			this,
			menu_selector(TreasureRoomLayer::onChestButton));
		button->setID("Treasure-Checklist-Button");
		
		menu->setPosition({winSize.width - 30.0f, winSize.height - 60.0f});
		menu->addChild(button);
		menu->updateLayout();

		this->addChild(menu);
		return true;
	}

	void onChestButton(CCObject *){
		TreasureCheckAlert::create()->show();
	}
};