# Treasure Checklist (GEODE MOD)

First Mod I programmed for Geometry Dash, publicly available on the Geode Modloader.

## Settings

* **Show Miscellaneous Rewards**: Displays rewards that aren't icons (like colors and death effects). This option is enabled by default.
* **Group by Chests**: All rewards you get from the Tier II chests are grouped instead of being displayed separately. This option is enabled by default.
* **Garage Button**: Removes the button of the mod from the Garage layer (Icon kit) so you can only see it in the Treasure Room. This option is enabled by default.
* **Disable Checkmark**: Icons aren't greyed out and displayed without the checkmark, if you want to see all the designs. This option is disabled by default.

## Developer Commentary

* The Mod hooks two layers in the game, GarageLayer (Icon Kit) and the SecretRewardsLayer (Treasure Room)
* In the Icon Kit, the mod adds a button in the "Shards-menu" node
* Most of the buttons, menus and sprites have an ID
* Portion of the code is commented, but not all
* Requires "Node-IDs" (v1.12.0 or above)

## To-do List

* Optimize the code, to reduce lag spikes
* MacOS compatibility
* Extra details
