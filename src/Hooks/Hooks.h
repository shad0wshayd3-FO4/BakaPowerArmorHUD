#pragma once

#include "MCM/MCM.h"
#include "Menus/PowerArmorConditionMenu.h"

class Hooks
{
public:
	static void Install()
	{
		hkGetPowerArmorShouldBeVisible<1402809, 0x1D>::Install();   // HUDActionPointMeter::CanPlaySoundEffects
		hkGetPowerArmorShouldBeVisible<742970, 0x190>::Install();   // HUDActiveEffectsDisplay::UpdateDisplayObject
		hkGetPowerArmorShouldBeVisible<756668, 0x5D9>::Install();   // HUDCompassMarker::SetCompassMarkerData
		hkGetPowerArmorShouldBeVisible<259269, 0xB6>::Install();    // HUDMarkerData::GetMinorFrame
		hkGetPowerArmorShouldBeVisible<1511766, 0x107>::Install();  // PowerArmorGeometry::UpdatePowerArmorHUD
		hkSetPowerArmorMode<1187476, 0xE6B>::Install();             // PowerArmor::SwitchToPowerArmor
		hkSetPowerArmorMode<1150710, 0x1E8>::Install();             // PowerArmor::SwitchFromPowerArmorFurnitureLoaded
		hkQActorInPowerArmor<31293, 0x14C, false>::Install();       // HUDRadiationModel::CalcEnvDamage
		hkQActorInPowerArmor<733718, 0xA8, false>::Install();       // HUDMenu::InitializeHUD
		hkQActorInPowerArmorPAHC<34363, 0x2D>::Install();           // HUDMenuUtils::GetGameplayHUDColor
		hkCanBeVisible<893789, 0x12>::Install();                    // HUDCompass::CanBeVisible
		hkGetPowerArmorHUDColor<523665, 0x56>::Install();           // GameUIModel::SetGameColors
	}

	static void InstallPostLoad()
	{
		MCM::Settings::Update();
		Menus::Register();

		if (auto PlayerCharacter = RE::PlayerCharacter::GetSingleton())
		{
			if (auto EventSource =
			        static_cast<
						RE::BSTEventSource<
							RE::ActorValueEvents::ActorValueChangedEvent>*>(
						PlayerCharacter))
			{
				EventSource->RegisterSink(ActorValueChangedHandler::GetSingleton());
			}
		}
	}

private:
	class detail
	{
	public:
		static bool IsExempt()
		{
			if (!MCM::Settings::General::bEnable.GetValue())
			{
				return true;
			}

			return false;
		}

		template <class T>
		static void Notify()
		{
			if (auto EventSource = T::GetEventSource())
			{
				auto eventData = T{};
				EventSource->Notify(eventData);
			}
		}

		template <class T>
		static void Notify(bool a_value)
		{
			if (auto EventSource = T::GetEventSource())
			{
				auto eventData = T{ a_value };
				EventSource->Notify(eventData);
			}
		}
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF>
	class hkGetPowerArmorShouldBeVisible
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_GetPowerArmorShouldBeVisible = trampoline.write_call<5>(target.address(), GetPowerArmorShouldBeVisible);
		}

	private:
		static bool GetPowerArmorShouldBeVisible(
			[[maybe_unused]] RE::PowerArmorGeometry* a_this)
		{
			if (detail::IsExempt())
			{
				return _GetPowerArmorShouldBeVisible(a_this);
			}

			return false;
		}

		inline static REL::Relocation<decltype(&GetPowerArmorShouldBeVisible)> _GetPowerArmorShouldBeVisible;
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF>
	class hkSetPowerArmorMode
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_SetPowerArmorMode = trampoline.write_call<5>(target.address(), SetPowerArmorMode);
		}

	private:
		static void SetPowerArmorMode(
			[[maybe_unused]] bool a_inPowerArmor)
		{
			if (detail::IsExempt())
			{
				return _SetPowerArmorMode(a_inPowerArmor);
			}

			if (MCM::Settings::General::bDisableColor.GetValue())
			{
				return;
			}

			detail::Notify<RE::ColorUpdateEvent>();
		}

		inline static REL::Relocation<decltype(&SetPowerArmorMode)> _SetPowerArmorMode;
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF, bool RETN>
	class hkQActorInPowerArmor
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_QActorInPowerArmor = trampoline.write_call<5>(target.address(), QActorInPowerArmor);
		}

	private:
		static bool QActorInPowerArmor(
			[[maybe_unused]] RE::Actor* a_actor)
		{
			if (detail::IsExempt())
			{
				return _QActorInPowerArmor(a_actor);
			}

			return RETN;
		}

		inline static REL::Relocation<decltype(&hkQActorInPowerArmor::QActorInPowerArmor)> _QActorInPowerArmor;
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF>
	class hkQActorInPowerArmorPAHC
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_QActorInPowerArmor = trampoline.write_call<5>(target.address(), QActorInPowerArmor);
		}

	private:
		static bool QActorInPowerArmor(
			[[maybe_unused]] RE::Actor* a_actor)
		{
			if (detail::IsExempt())
			{
				return _QActorInPowerArmor(a_actor);
			}

			if (MCM::Settings::General::bDisableColor.GetValue())
			{
				return _QActorInPowerArmor(a_actor);
			}

			if (!MCM::Settings::General::bDisablePAColor.GetValue())
			{
				return _QActorInPowerArmor(a_actor);
			}

			return false;
		}

		inline static REL::Relocation<decltype(&QActorInPowerArmor)> _QActorInPowerArmor;
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF>
	class hkCanBeVisible
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_CanBeVisible = trampoline.write_call<5>(target.address(), CanBeVisible);
		}

	private:
		static bool CanBeVisible(
			[[maybe_unused]] void* a_this)
		{
			if (detail::IsExempt())
			{
				return _CanBeVisible(a_this);
			}

			auto bCanBeVisible = _CanBeVisible(a_this);
			if (bCanBeVisible && RE::PowerArmor::PlayerInPowerArmor())
			{
				Menus::PowerArmorConditionMenu::ShowMenu();
			}
			else
			{
				Menus::PowerArmorConditionMenu::HideMenu();
			}

			return bCanBeVisible;
		}

		inline static REL::Relocation<decltype(&CanBeVisible)> _CanBeVisible;
	};

	template <std::uint64_t ID, std::ptrdiff_t OFF>
	class hkGetPowerArmorHUDColor
	{
	public:
		static void Install()
		{
			static REL::Relocation<std::uintptr_t> target{ REL::ID(ID), OFF };
			auto& trampoline = F4SE::GetTrampoline();
			_GetPowerArmorHUDColor = trampoline.write_call<5>(target.address(), GetPowerArmorHUDColor);
		}

	private:
		static RE::NiColor GetPowerArmorHUDColor()
		{
			if (detail::IsExempt())
			{
				return _GetPowerArmorHUDColor();
			}

			if (MCM::Settings::General::bDisableColor.GetValue())
			{
				return _GetPowerArmorHUDColor();
			}

			if (!MCM::Settings::General::bDisablePAColor.GetValue())
			{
				return _GetPowerArmorHUDColor();
			}

			return GetGameplayHUDColor();
		}

		static RE::NiColor GetGameplayHUDColor()
		{
			using func_t = decltype(&GetGameplayHUDColor);
			REL::Relocation<func_t> func{ REL::ID(34363) };
			return func();
		}

		inline static REL::Relocation<decltype(&GetPowerArmorHUDColor)> _GetPowerArmorHUDColor;
	};

	class ActorValueChangedHandler :
		public RE::BSTEventSink<RE::ActorValueEvents::ActorValueChangedEvent>
	{
	public:
		[[nodiscard]] static ActorValueChangedHandler* GetSingleton()
		{
			static ActorValueChangedHandler singleton;
			return std::addressof(singleton);
		}

		virtual RE::BSEventNotifyControl ProcessEvent(
			const RE::ActorValueEvents::ActorValueChangedEvent& a_event,
			RE::BSTEventSource<RE::ActorValueEvents::ActorValueChangedEvent>*) override
		{
			if (detail::IsExempt())
			{
				return RE::BSEventNotifyControl::kContinue;
			}

			auto ActorValue = RE::ActorValue::GetSingleton();
			if (!ActorValue)
			{
				return RE::BSEventNotifyControl::kContinue;
			}

			switch (a_event.actorValue.formID)
			{
			case 0x0000035C:  // PowerArmorBattery
				if (!RE::PowerArmor::PlayerInPowerArmor())
				{
					detail::Notify<RE::PowerArmorLightData>(false);
				}
				break;

			default:
				break;
			}

			return RE::BSEventNotifyControl::kContinue;
		}
	};
};
