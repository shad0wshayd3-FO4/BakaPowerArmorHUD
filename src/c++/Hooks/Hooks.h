#pragma once

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
	}

private:
	class detail
	{
	public:
		static bool IsExempt()
		{
			return false;
		}
	};

	template<std::uint64_t ID, std::ptrdiff_t OFF>
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
				return GetPowerArmorShouldBeVisible(a_this);
			}

			return false;
		}

		inline static REL::Relocation<decltype(&GetPowerArmorShouldBeVisible)> _GetPowerArmorShouldBeVisible;
	};

	template<std::uint64_t ID, std::ptrdiff_t OFF>
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
				return SetPowerArmorMode(a_inPowerArmor);
			}

			if (auto EventSource = RE::ColorUpdateEvent::GetEventSource())
			{
				auto colorUpdateEvent = RE::ColorUpdateEvent{};
				EventSource->Notify(colorUpdateEvent);
			}

			if (auto EventSource = RE::PowerArmorLightData::GetEventSource())
			{
				auto powerArmorLightData = RE::PowerArmorLightData{ true };
				EventSource->Notify(powerArmorLightData);
			}
		}

		inline static REL::Relocation<decltype(&SetPowerArmorMode)> _SetPowerArmorMode;
	};

	template<std::uint64_t ID, std::ptrdiff_t OFF, bool RETN>
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
				return QActorInPowerArmor(a_actor);
			}

			return RETN;
		}

		inline static REL::Relocation<decltype(&hkQActorInPowerArmor::QActorInPowerArmor)> _QActorInPowerArmor;
	};
};
