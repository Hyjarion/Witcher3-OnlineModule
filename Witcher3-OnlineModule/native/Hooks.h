#include "dep/subhook/subhook.h"
#include "lib/MinHook.h"
#include <cuchar>

#pragma comment(lib, "lib/libMinHook.x64.lib")

namespace Hooks
{
    /*   
        Issues/observations:

        - (Observed with Subhook) Debug mode only: Passing a string argument into a function (that i called from the hook) causes funky stuff ingame for some reason (pass a char array pointer instead to avoid this)
          maybe this is only happening via u16string only.

        - Runtime checks can fuck up hooks (maybe this is cause of the above issue)

    */
    
    typedef void(*_CWorldTick)(uintptr_t CTickManager, uintptr_t CWorldTickInfo, uintptr_t CRenderFrameInfo, uintptr_t CRenderFrame);
    subhook::Hook WorldTick_subhook;
#pragma runtime_checks( "", off )
    void CWorldTick_Hook(uintptr_t CTickManager, uintptr_t CWorldTickInfo, uintptr_t CRenderFrameInfo, uintptr_t CRenderFrame)
    {
        if (FunctionThreadDelayer::func_SpawnPlayer_delays.size())
        {
            for (FunctionThreadDelayer::func_SpawnPlayer i : FunctionThreadDelayer::func_SpawnPlayer_delays)
            {
                uintptr_t EntityAddress = NativeHelper::CreateEntitySyncByResourceName(i.ResName, i.pos, i.isDepotPath);
                PlayerDataList.push_back({ i.ID, EntityAddress });

                if (i.isDepotPath && (i.ResName == u"characters\\npc_entities\\main_npc\\geralt_npc.w2ent"))
                {
                    std::string command = "SetupGeraltFor(" + std::to_string(Natives::CEntity::GetGuidHash(EntityAddress)) + ")";
                    NativeHelper::ExecGameConsoleCommand(command);
                }
            }
            FunctionThreadDelayer::func_SpawnPlayer_delays.clear();
        }

        if (FunctionThreadDelayer::func_SpawnNpc_delays.size())
        {
            for (FunctionThreadDelayer::func_SpawnNpc i : FunctionThreadDelayer::func_SpawnNpc_delays)
            {
                uintptr_t EntityAddress = NativeHelper::CreateEntitySyncByResourceName(i.ResName, i.pos, false);
                NpcDataList.push_back({ i.ID, EntityAddress });

                if (uintptr_t W3AbilityManager = Natives::CActor::GetW3AbilityManager(EntityAddress))
                {
                    Natives::W3AbilityManager::SetStatPointMax(W3AbilityManager, EBaseCharacterStats::BCS_Essence, 500.f);
                    Natives::W3AbilityManager::SetStatPointCurrent(W3AbilityManager, EBaseCharacterStats::BCS_Essence, 500.f);
                }
            }
            FunctionThreadDelayer::func_SpawnNpc_delays.clear();
        }

        if (FunctionThreadDelayer::func_MoveActorTo_delays.size())
        {
            for (FunctionThreadDelayer::func_MoveActorTo i : FunctionThreadDelayer::func_MoveActorTo_delays)
            {
                /*Natives::CActor::ActionCancelAll(i.CActor);*/
                Natives::CActor::ActionMoveTo(i.CActor, i.pos, i.EMoveType, i.absSpeed, i.radius, i.EMoveFailureAction, i.unk, i.unk2);
            }
            FunctionThreadDelayer::func_MoveActorTo_delays.clear();
        }

        if (FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.size())
        {
            FunctionThreadDelayer::func_ExecGameConsoleCommand_locked = true;
            for (FunctionThreadDelayer::func_ExecGameConsoleCommand i : FunctionThreadDelayer::func_ExecGameConsoleCommand_delays)
                NativeHelper::ExecGameConsoleCommand(i.command);
            FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.clear();
            FunctionThreadDelayer::func_ExecGameConsoleCommand_locked = false;
        }

        ((_CWorldTick)WorldTick_subhook.GetTrampoline())(CTickManager, CWorldTickInfo, CRenderFrameInfo, CRenderFrame);
    }
#pragma runtime_checks( "", restore )

    typedef void (*_TStringLeftString)(uintptr_t string, uintptr_t result, uintptr_t length);
    //_TStringLeftString TStringLeftString_trmp = 0;
    subhook::Hook TStringLeftString_subhook;
#pragma runtime_checks( "", off )
    void TStringLeftString_Hook(uintptr_t string, uintptr_t result, uintptr_t length)
    {
        char16_t* chars = nullptr;

        if (uintptr_t a1 = *reinterpret_cast<uintptr_t*>(string))
            chars = reinterpret_cast<char16_t*>(a1);

        if (chars != nullptr)
        {
            char16_t compare[] = u"SM:";

            uint8 matchedchars = 0;
            uint8 i = 0;
            while (chars[i] != 0 || i < 2)
            {
                if (chars[i] == compare[i])
                    ++matchedchars;
                else break;
                ++i;
            }

            if (matchedchars == 3)
            {
                switch(chars[5])
                {
                    // damaged enemy
                    case u'1':
                    {
                        if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                        {
                            if (uintptr_t Target = Natives::CActor::GetTarget(Player))
                            {
                                NpcData found;
                                for (NpcData i : NpcDataList)
                                {
                                    if (i.EntityAddress == Target)
                                    {
                                        found = i;
                                        break;
                                    }
                                }

                                if (found.EntityAddress)
                                {
                                    Networking::message<MessageTypes> msg;
                                    msg.header.id = MessageTypes::TS_HIT_NPC;
                                    msg << found.ID;

                                    w3client->MessageServer(msg);
                                }
                                else
                                {
                                    uint32 targetGuidHash = Natives::CEntity::GetGuidHash(Target);
                                    std::string command = "KillActorByGuidHash(" + std::to_string(targetGuidHash) + ")";
                                    FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });

                                }
                            }
                        }
                        break;
                    }

                    // got hit by enemy
                    case u'2':
                    {
                        Networking::message<MessageTypes> msg;
                        msg.header.id = MessageTypes::TS_GOT_HIT;
                        w3client->MessageServer(msg);
                        break;
                    }
                }
            }
        }

        if (chars != nullptr)
        {
            char16_t compare[] = u"SC:";

            uint8 matchedchars = 0;
            uint8 i = 0;
            while (chars[i] != 0 || i < 2)
            {
                if (chars[i] == compare[i])
                    ++matchedchars;
                else break;
                ++i;
            }

            if (matchedchars == 3 && (chars[3] != 0))
            {
                std::u16string chat16 = &chars[3];
                std::string chat = Database::ResourceNames::ToU8(chat16);

                if (chat.length() <= 100)
                {
                    Networking::message<MessageTypes> msg;
                    msg.header.id = MessageTypes::TS_CHAT_MESSAGE;

                    for (char i : chat)
                        msg << i;

                    w3client->MessageServer(msg);
                }
            }
        }

        ((_TStringLeftString)TStringLeftString_subhook.GetTrampoline())(string, result, length);
        //TStringLeftString_trmp(string, result, length);
    }
}
#pragma runtime_checks( "", restore )

void InstallHooks()
{
    Hooks::_CWorldTick CWorldTick = (Hooks::_CWorldTick)(baseAddress + 0x21C460);
    Hooks::WorldTick_subhook.Install(CWorldTick, (void*)Hooks::CWorldTick_Hook, subhook::HookFlags::HookFlag64BitOffset);

    Hooks::_TStringLeftString TStringLeftString = (Hooks::_TStringLeftString)(baseAddress + 0x2AC50);
    Hooks::TStringLeftString_subhook.Install(TStringLeftString, (void*)Hooks::TStringLeftString_Hook, subhook::HookFlags::HookFlag64BitOffset);

     //MH_Initialize();
     //MH_CreateHook((LPVOID)(baseAddress + 0x2AC50), Hooks::TStringLeftString_Hook, reinterpret_cast<LPVOID*>(&Hooks::TStringLeftString_trmp));
     //MH_EnableHook(MH_ALL_HOOKS);
}


