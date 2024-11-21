#pragma once

#include "MCM/MCM.h"

namespace Papyrus
{
	namespace BakaPowerArmorHUD
	{
		static constexpr auto SCRIPT_NAME{ "BakaPowerArmorHUD"sv };

		void UpdateSettings(std::monostate)
		{
			MCM::Settings::Update();
		}
	}

	bool RegisterFunctions(RE::BSScript::IVirtualMachine* a_VM)
	{
		// BakaPowerArmorHUD
		a_VM->BindNativeMethod(BakaPowerArmorHUD::SCRIPT_NAME, "UpdateSettings", BakaPowerArmorHUD::UpdateSettings, true);

		return true;
	}
}
