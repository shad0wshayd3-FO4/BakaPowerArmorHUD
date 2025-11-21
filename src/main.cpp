#include "Hooks/Hooks.h"
#include "MCM/MCM.h"
#include "Menus/PowerArmorConditionMenu.h"
#include "Scripts/Papyrus.h"

namespace
{
	void MessageCallback(F4SE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case F4SE::MessagingInterface::kGameDataReady:
			Hooks::Install();
			break;
		default:
			break;
		}
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se, { .trampoline = true, .trampolineSize = 128 });

	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
