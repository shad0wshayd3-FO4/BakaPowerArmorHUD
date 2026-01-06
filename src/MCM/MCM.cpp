#include "MCM/MCM.h"

#include "Menus/PowerArmorConditionMenu.h"

namespace MCM
{
	void Settings::PosUpdate()
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

	void Settings::PreUpdate()
	{
		if (bRegistered)
		{
			return;
		}

		if (auto UI = RE::UI::GetSingleton())
		{
			UI->RegisterSink<RE::MenuOpenCloseEvent>(EventHandler::GetSingleton());
			bRegistered = true;
		}
	}
}
