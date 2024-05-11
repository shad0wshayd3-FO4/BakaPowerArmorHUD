#include "MCM/MCM.h"

#include "Menus/PowerArmorConditionMenu.h"

namespace MCM
{
	void Settings::ResetStatePost()
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

		m_ini_base.Reset();
		m_ini_user.Reset();
	}
}
