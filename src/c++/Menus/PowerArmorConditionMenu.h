#pragma once

#include "MCM/MCM.h"

namespace Menus
{
	class PowerArmorConditionMenu :
		public RE::GameMenuBase
	{
	public:
		static constexpr auto MENU_NAME{ "PowerArmorConditionMenu"sv };

		PowerArmorConditionMenu()
		{
			menuFlags.set(
				RE::UI_MENU_FLAGS::kAllowSaving,
				RE::UI_MENU_FLAGS::kCustomRendering);
			depthPriority.set(RE::UI_DEPTH_PRIORITY::kHUD);

			inputEventHandlingEnabled = false;

			auto MoviePath = "Interface\\PowerArmorConditionMenu.swf"sv;
			const auto ScaleformManager = RE::BSScaleformManager::GetSingleton();
			[[maybe_unused]] const auto LoadMovieSuccess =
				ScaleformManager->LoadMovieEx(*this, MoviePath, "root");
			assert(LoadMovieSuccess);

			filterHolder = RE::msvc::make_unique<RE::BSGFxShaderFXTarget>(*uiMovie, "root.Menu_mc");
			if (filterHolder)
			{
				shaderFXObjects.push_back(filterHolder.get());
			}

			ConditionMeter_mc = RE::msvc::make_unique<RE::BSGFxShaderFXTarget>(*filterHolder, "ConditionMeter_mc");
			if (ConditionMeter_mc)
			{
				ConditionMeter_mc->CreateAndSetFiltersToHUD(RE::HUDColorTypes::kPlayerSetColor);
				shaderFXObjects.push_back(ConditionMeter_mc.get());
			}
		}

		virtual ~PowerArmorConditionMenu()
		{
			ConditionMeter_mc.release();
		}

		virtual RE::UI_MESSAGE_RESULTS ProcessMessage(RE::UIMessage& a_message) override
		{
			switch (*a_message.type)
			{
				case RE::UI_MESSAGE_TYPE::kShow:
					SetConditionMeterVisuals(
						static_cast<float>(MCM::Settings::General::fConditionMeterX),
						static_cast<float>(MCM::Settings::General::fConditionMeterY),
						static_cast<float>(MCM::Settings::General::fConditionMeterScale));
					UpdateBatteryState();
					
				default:
					return __super::ProcessMessage(a_message);
			}
		}

		static RE::IMenu* Create(const RE::UIMessage&)
		{
			return new PowerArmorConditionMenu();
		}

		static void ShowMenu()
		{
			if (auto UI = RE::UI::GetSingleton())
			{
				if (UI->GetMenuOpen<PowerArmorConditionMenu>())
				{
					return;
				}
			}
			else
			{
				return;
			}

			if (!RE::PowerArmor::PlayerInPowerArmor())
			{
				return;
			}

			if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
			{
				UIMessageQueue->AddMessage(
					"PowerArmorConditionMenu"sv,
					RE::UI_MESSAGE_TYPE::kShow);
			}
		}

		static void HideMenu()
		{
			if (auto UI = RE::UI::GetSingleton())
			{
				if (!UI->GetMenuOpen<PowerArmorConditionMenu>())
				{
					return;
				}
			}
			else
			{
				return;
			}

			if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
			{
				UIMessageQueue->AddMessage(
					"PowerArmorConditionMenu"sv,
					RE::UI_MESSAGE_TYPE::kHide);
			}
		}

		static void SetConditionMeterVisuals(float a_x, float a_y, float a_scale)
		{
			if (auto UI = RE::UI::GetSingleton())
			{
				if (auto Menu = UI->GetMenu<PowerArmorConditionMenu>())
				{
					Menu->SetConditionMeterVisualsImpl(a_x, a_y, a_scale);
				}
			}
		}

		static void UpdateBatteryState()
		{
			if (auto UI = RE::UI::GetSingleton())
			{
				if (auto Menu = UI->GetMenu<PowerArmorConditionMenu>())
				{
					Menu->UpdateBatteryStateImpl();
				}
			}
		}

	private:
		class detail
		{
		public:
			template<class T>
			static void Notify(bool a_value)
			{
				if (auto EventSource = T::GetEventSource())
				{
					auto eventData = T{ a_value };
					EventSource->Notify(eventData);
				}
			}
		};

		void SetConditionMeterCountImpl(std::uint32_t a_count)
		{
			RE::Scaleform::GFx::Value args[1];
			args[0] = a_count;
			ConditionMeter_mc->Invoke("SetCount", nullptr, args, 1);
		}

		void SetConditionMeterPercentImpl(float a_percent)
		{
			RE::Scaleform::GFx::Value args[1];
			args[0] = a_percent;
			ConditionMeter_mc->Invoke("SetPercent", nullptr, args, 1);
			ConditionMeter_mc->SetToHUDColor(a_percent <= fPowerArmorLowBatterySoundThreshold->GetFloat());
		}

		void SetConditionMeterVisualsImpl(float a_x, float a_y, float a_scale)
		{
			ConditionMeter_mc->SetMember("x", a_x);
			ConditionMeter_mc->SetMember("y", a_y);
			ConditionMeter_mc->SetMember("scaleX", a_scale);
			ConditionMeter_mc->SetMember("scaleY", a_scale);
		}

		void UpdateBatteryStateImpl()
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				std::uint32_t count = PlayerCharacter->GetInventoryObjectCount(RE::PowerArmor::GetDefaultBatteryObject());
				SetConditionMeterCountImpl(count);

				if (auto ActorValue = RE::ActorValue::GetSingleton())
				{
					float value = PlayerCharacter->GetActorValue(*ActorValue->powerArmorBattery);
					if (value <= 0.0f)
					{
						detail::Notify<RE::PowerArmorLightData>(true);
					}
					else
					{
						detail::Notify<RE::PowerArmorLightData>(false);
					}

					SetConditionMeterPercentImpl(value);
				}
			}
		}

		RE::msvc::unique_ptr<RE::BSGFxShaderFXTarget> ConditionMeter_mc;

	protected:
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> fPowerArmorLowBatterySoundThreshold{ REL::ID(370701) };
	};

	inline void Register()
	{
		if (auto UI = RE::UI::GetSingleton())
		{
			UI->RegisterMenu(
				"PowerArmorConditionMenu",
				PowerArmorConditionMenu::Create);
		}
	}
}
