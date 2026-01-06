#pragma once

#include "MCM/MCM.h"
#include "Menus/PowerArmorConditionMenu.h"

class Hooks
{
private:
	class detail
	{
	public:
		static bool IsExempt()
		{
			if (!MCM::Settings::General::bEnable)
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
	};

	class hkUpdatePowerArmorHUD
	{
	private:
		static void UpdatePowerArmorHUD(
			[[maybe_unused]] RE::PowerArmorGeometry* a_this)
		{
			if (detail::IsExempt())
			{
				return _UpdatePowerArmorHUD0(a_this);
			}

			if (MCM::Settings::Runtime::bRunOnce)
			{
				MCM::Settings::Runtime::bRunOnce = false;
				return _UpdatePowerArmorHUD0(a_this);
			}

			return;
		}

		inline static REL::Hook _UpdatePowerArmorHUD0{ REL::ID(2248849), 0x10, UpdatePowerArmorHUD };  // HUDMenu::Update (unused?)
		inline static REL::Hook _UpdatePowerArmorHUD1{ REL::ID(2248853), 0x4E, UpdatePowerArmorHUD };  // HUDMenu::AdvanceMovie
	};

	class hkGetPowerArmorShouldBeVisible
	{
	private:
		static bool GetPowerArmorShouldBeVisible(
			[[maybe_unused]] RE::PowerArmorGeometry* a_this)
		{
			if (detail::IsExempt())
			{
				return _GetPowerArmorShouldBeVisible0(a_this);
			}

			return false;
		}

		inline static REL::Hook _GetPowerArmorShouldBeVisible0{ REL::ID(2219884), 0x01D, GetPowerArmorShouldBeVisible };  // HUDActionPointMeter::CanPlaySoundEffects
		inline static REL::Hook _GetPowerArmorShouldBeVisible1{ REL::ID(2219923), 0x269, GetPowerArmorShouldBeVisible };  // HUDActiveEffectsDisplay::UpdateDisplayObject
		inline static REL::Hook _GetPowerArmorShouldBeVisible2{ REL::ID(2220221), 0x56C, GetPowerArmorShouldBeVisible };  // HUDCompassMarker::SetCompassMarkerData
		inline static REL::Hook _GetPowerArmorShouldBeVisible3{ REL::ID(2220500), 0x109, GetPowerArmorShouldBeVisible };  // HUDMarkerData::GetMinorFrame
		// inline static REL::Hook _GetPowerArmorShouldBeVisible4{ REL::ID(2248874), 0x1ED, GetPowerArmorShouldBeVisible };  // PowerArmorGeometry::UpdatePowerArmorHUD
	};

	class hkSetPowerArmorMode
	{
	private:
		static void SetPowerArmorMode(
			[[maybe_unused]] bool a_inPowerArmor)
		{
			if (detail::IsExempt())
			{
				return _SetPowerArmorMode0(a_inPowerArmor);
			}

			detail::Notify<RE::ColorUpdateEvent>();
		}

		inline static REL::Hook _SetPowerArmorMode0{ REL::ID(2219442), 0xC8A, SetPowerArmorMode };  // PowerArmor::SwitchToPowerArmor
		inline static REL::Hook _SetPowerArmorMode1{ REL::ID(2219448), 0x22F, SetPowerArmorMode };  // PowerArmor::SwitchFromPowerArmorFurnitureLoaded
	};

	class hkQActorInPowerArmor
	{
	private:
		static bool QActorInPowerArmor(
			[[maybe_unused]] RE::Actor* a_actor)
		{
			if (detail::IsExempt())
			{
				return _QActorInPowerArmor0(a_actor);
			}

			return false;
		}

		inline static REL::Hook _QActorInPowerArmor0{ REL::ID(2221907), 0x170, QActorInPowerArmor };  // HUDRadiationModel::CalculateEnvironmentalDamage
		inline static REL::Hook _QActorInPowerArmor1{ REL::ID(2248856), 0x0C1, QActorInPowerArmor };  // HUDMenu::InitializeHUD
	};

	class hkQActorInPowerArmor_HUDColor
	{
	private:
		static bool QActorInPowerArmor(
			[[maybe_unused]] RE::Actor* a_actor)
		{
			if (detail::IsExempt())
			{
				return _QActorInPowerArmor0(a_actor);
			}

			if (!MCM::Settings::General::bDisablePAColor)
			{
				return _QActorInPowerArmor0(a_actor);
			}

			return false;
		}

		inline static REL::Hook _QActorInPowerArmor0{ REL::ID(2248840), 0x2D, QActorInPowerArmor };  // HUDMenuUtils::GetGameplayHUDColor
		inline static REL::Hook _QActorInPowerArmor1{ REL::ID(2248845), 0x33, QActorInPowerArmor };  // HUDMenuUtils::GetGameplayHUDBackgroundColor
	};

	class hkCanBeVisible
	{
	private:
		static bool CanBeVisible(
			[[maybe_unused]] void* a_this)
		{
			if (detail::IsExempt())
			{
				return _CanBeVisible0(a_this);
			}

			auto bCanBeVisible = _CanBeVisible0(a_this);
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

		inline static REL::Hook _CanBeVisible0{ REL::ID(2220160), 0x0A, CanBeVisible };  // HUDCompass::CanBeVisible
	};

	class hkGetPowerArmorHUDColor
	{
	private:
		static RE::NiColor GetGameplayHUDColor()
		{
			using func_t = decltype(&GetGameplayHUDColor);
			REL::Relocation<func_t> func{ REL::ID(2248840) };
			return func();
		}

	private:
		static RE::NiColor GetPowerArmorHUDColor()
		{
			if (detail::IsExempt())
			{
				return _GetPowerArmorHUDColor0();
			}

			if (!MCM::Settings::General::bDisablePAColor)
			{
				return _GetPowerArmorHUDColor0();
			}

			return GetGameplayHUDColor();
		}

		inline static REL::Hook _GetPowerArmorHUDColor0{ REL::ID(2220911), 0x48, GetPowerArmorHUDColor };  // GameUIModel::SetGameColors
	};
};
