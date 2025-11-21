#include "MCM/MCM.h"

#include "Menus/PowerArmorConditionMenu.h"

namespace MCM::Settings
{
	void Reset()
	{
		if (RE::PowerArmor::PlayerInPowerArmor())
		{
			if (General::bEnable)
			{
				RE::SendHUDMessage::SetPowerArmorMode(false);
				Menus::PowerArmorConditionMenu::ShowMenu();
			}
			else
			{
				RE::SendHUDMessage::SetPowerArmorMode(true);
				Menus::PowerArmorConditionMenu::HideMenu();
			}
		}
	}

	void Update()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"Data/MCM/Config/BakaPowerArmorHUD/settings.ini",
			"Data/MCM/Settings/BakaPowerArmorHUD.ini");
		ini->Load();

		Reset();
	}
}
