#pragma once

namespace MCM
{
	class Settings
	{
	public:
		class General
		{
		public:
			inline static REX::INI::Bool bEnable{ "General", "bEnable", true };
			inline static REX::INI::Bool bDisablePAColor{ "General", "bDisablePAColor", false };

			inline static REX::INI::F32 fConditionMeterX{ "General", "fConditionMeterX", 136.0f };
			inline static REX::INI::F32 fConditionMeterY{ "General", "fConditionMeterY", 675.0f };
			inline static REX::INI::F32 fConditionMeterScale{ "General", "fConditionMeterScale", 1.0f };
		};

		static void Update()
		{
			const auto ini = REX::INI::SettingStore::GetSingleton();
			ini->Init(
				"Data/MCM/Config/BakaPowerArmorHUD/settings.ini",
				"Data/MCM/Settings/BakaPowerArmorHUD.ini");
			ini->Load();

			PostUpdate();
		}

	private:
		static void PostUpdate();
	};
}
