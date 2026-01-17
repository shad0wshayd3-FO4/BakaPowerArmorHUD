#include "MCM/MCM.h"

#include "Menus/PowerArmorConditionMenu.h"

namespace MCM
{
	namespace detail
	{
		template <class T>
		static void Notify()
		{
			if (auto EventSource = T::GetEventSource())
			{
				auto eventData = T{};
				EventSource->Notify(eventData);
			}
		}
	}

	void Settings::PosUpdate()
	{
		if (RE::PowerArmor::PlayerInPowerArmor())
		{
			if (Runtime::bEnable != General::bEnable)
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

				Runtime::bRunOnce = true;
			}

			if (auto UI = RE::UI::GetSingleton();
				UI && !UI->GetMenuOpen<RE::PipboyMenu>())
			{
				detail::Notify<RE::ColorUpdateEvent>();
			}
		}
	}

	void Settings::PreUpdate()
	{
		Runtime::bEnable = General::bEnable;

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
