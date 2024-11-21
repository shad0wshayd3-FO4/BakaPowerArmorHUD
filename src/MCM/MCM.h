#pragma once

namespace MCM::Settings
{
	namespace General
	{
		static REX::INI::Bool bEnable{ "General", "bEnable", true };
		static REX::INI::Bool bDisablePAColor{ "General", "bDisablePAColor", false };
		static REX::INI::Bool bDisableColor{ "General", "bDisableColor", false };

		static REX::INI::F64 fConditionMeterX{ "General", "fConditionMeterX", 136.0 };
		static REX::INI::F64 fConditionMeterY{ "General", "fConditionMeterY", 675.0 };
		static REX::INI::F64 fConditionMeterScale{ "General", "fConditionMeterScale", 1.0 };
	}

	static void Reset();

	static void Update()
	{
		const auto ini = REX::INI::SettingStore::GetSingleton();
		ini->Init(
			"Data/MCM/Config/BakaPowerArmorHUD/settings.ini",
			"Data/MCM/Settings/BakaPowerArmorHUD.ini");
		ini->Load();

		Reset();
	}
}
