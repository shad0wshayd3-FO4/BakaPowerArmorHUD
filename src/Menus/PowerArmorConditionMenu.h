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
				RE::UI_MENU_FLAGS::kDontHideCursorWhenTopmost,
				RE::UI_MENU_FLAGS::kCustomRendering);
			depthPriority = RE::UI_DEPTH_PRIORITY::kSWFLoader;

			auto MoviePath = "Interface\\PowerArmorConditionMenu.swf"sv;
			const auto ScaleformManager = RE::BSScaleformManager::GetSingleton();
			[[maybe_unused]] const auto LoadMovieSuccess =
				ScaleformManager->LoadMovieEx(*this, MoviePath, "root");
			assert(LoadMovieSuccess);

			filterHolder = std::make_unique<RE::BSGFxShaderFXTarget>(*uiMovie, "root.Menu_mc.ConditionMeter_mc");
			if (filterHolder)
			{
				filterHolder->CreateAndSetFiltersToHUD(RE::HUDColorTypes::kPlayerSetColor);
				shaderFXObjects.push_back(filterHolder.get());
			}
		}

		static RE::IMenu* Create(const RE::UIMessage&)
		{
			return new PowerArmorConditionMenu();
		}

		static void ShowMenu()
		{
			if (!RE::PowerArmor::PlayerInPowerArmor())
			{
				return;
			}

			if (auto UI = RE::UI::GetSingleton())
			{
				if (UI->GetMenuOpen<PowerArmorConditionMenu>())
				{
					SetConditionMeterVisuals(
						MCM::Settings::General::fConditionMeterX,
						MCM::Settings::General::fConditionMeterY,
						MCM::Settings::General::fConditionMeterScale);
					UpdateBatteryState();
					return;
				}
			}
			else
			{
				return;
			}

			if (auto UIMessageQueue = RE::UIMessageQueue::GetSingleton())
			{
				UIMessageQueue->AddMessage("PowerArmorConditionMenu"sv, RE::UI_MESSAGE_TYPE::kShow);
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
				UIMessageQueue->AddMessage("PowerArmorConditionMenu"sv, RE::UI_MESSAGE_TYPE::kHide);
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
		void SetConditionMeterCountImpl(std::uint32_t a_count)
		{
			Scaleform::GFx::Value args[1];
			args[0] = a_count;
			filterHolder->Invoke("SetCount", nullptr, args, 1);
		}

		void SetConditionMeterPercentImpl(float a_percent)
		{
			Scaleform::GFx::Value args[1];
			args[0] = a_percent;
			filterHolder->Invoke("SetPercent", nullptr, args, 1);
			filterHolder->SetToHUDColor(a_percent <= fPowerArmorLowBatterySoundThreshold->GetFloat());
		}

		void SetConditionMeterVisualsImpl(float a_x, float a_y, float a_scale)
		{
			filterHolder->SetMember("x", a_x);
			filterHolder->SetMember("y", a_y);
			filterHolder->SetMember("scaleX", a_scale);
			filterHolder->SetMember("scaleY", a_scale);
		}

		void UpdateBatteryStateImpl()
		{
			if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
			{
				auto count = PlayerCharacter->GetInventoryObjectCount(RE::PowerArmor::GetDefaultBatteryObject());
				SetConditionMeterCountImpl(count);

				if (auto ActorValue = RE::ActorValue::GetSingleton())
				{
					auto value = PlayerCharacter->GetActorValue(*ActorValue->powerArmorBattery);
					SetConditionMeterPercentImpl(value);
				}
			}
		}

	protected:
		inline static REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> fPowerArmorLowBatterySoundThreshold{ REL::ID(370701) };
	};

	inline void Register()
	{
		MCM::Settings::Update();

		if (auto UI = RE::UI::GetSingleton())
		{
			UI->RegisterMenu("PowerArmorConditionMenu", PowerArmorConditionMenu::Create);
		}
	}
}
