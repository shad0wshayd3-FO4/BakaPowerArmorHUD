#pragma once

namespace MCM::Settings
{
	namespace General
	{
		static REX::INI::Bool bEnable{ "General", "bEnable", true };
		static REX::INI::Bool bDisablePAColor{ "General", "bDisablePAColor", false };
		static REX::INI::Bool bDisableColor{ "General", "bDisableColor", false };

		static REX::INI::F32 fConditionMeterX{ "General", "fConditionMeterX", 136.0f };
		static REX::INI::F32 fConditionMeterY{ "General", "fConditionMeterY", 675.0f };
		static REX::INI::F32 fConditionMeterScale{ "General", "fConditionMeterScale", 1.0f };
	}

	void Reset();
	void Update();
}
