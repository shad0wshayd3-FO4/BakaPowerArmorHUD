#include "MCM/MCM.h"

#include "Menus/PowerArmorConditionMenu.h"

namespace MCM::Settings
{
	void Reset()
	{
		if (RE::PowerArmor::PlayerInPowerArmor())
		{
			if (General::bEnable.GetValue())
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
}
