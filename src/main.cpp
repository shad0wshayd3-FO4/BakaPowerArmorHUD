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
		case F4SE::MessagingInterface::kPostLoad:
			Hooks::Install();
			break;
		case F4SE::MessagingInterface::kGameDataReady:
			Hooks::InstallPostLoad();
			break;
		default:
			break;
		}
	}
}

F4SEPluginLoad(const F4SE::LoadInterface* a_F4SE)
{
	F4SE::Init(a_F4SE);

	F4SE::AllocTrampoline(1u << 8);
	F4SE::GetMessagingInterface()->RegisterListener(MessageCallback);
	F4SE::GetPapyrusInterface()->Register(Papyrus::RegisterFunctions);

	return true;
}
