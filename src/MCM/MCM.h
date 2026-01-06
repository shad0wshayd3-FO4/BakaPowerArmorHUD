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

		class Runtime
		{
		public:
			inline static bool bEnable{ true };
		};

		static void Update()
		{
			PreUpdate();

			const auto ini = REX::INI::SettingStore::GetSingleton();
			ini->Init(
				"Data/MCM/Config/BakaPowerArmorHUD/settings.ini",
				"Data/MCM/Settings/BakaPowerArmorHUD.ini");
			ini->Load();

			PosUpdate();
		}

	private:
		class EventHandler :
			public RE::BSTEventSink<RE::MenuOpenCloseEvent>,
			public REX::Singleton<EventHandler>
		{
		public:
			virtual RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
			{
				if (a_event.menuName == "PauseMenu" && !a_event.opening)
				{
					MCM::Settings::Update();
				}

				return RE::BSEventNotifyControl::kContinue;
			}
		};

		static void PosUpdate();
		static void PreUpdate();

		inline static bool bRegistered{ false };
	};
}
