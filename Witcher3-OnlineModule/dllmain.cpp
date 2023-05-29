#include <chrono>
#include "pch.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <psapi.h>

#include "networking/net_message.h"
#include "networking/net_tsqueue.h"
#include "networking/net_connection.h"
#include "networking/net_client.h"
#include "networking/PacketTypes.h"

uintptr_t baseAddress = (uintptr_t)GetModuleHandle(NULL);

typedef signed char         int8;
typedef signed short        int16;
typedef signed int          int32;
typedef signed __int64      int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;

#include "database/ResourceNames.h"

struct PlayerData
{
    uint32 ID = 0;
    uintptr_t EntityAddress = 0;
    uint64 chatTimer = 0;
};
std::vector<PlayerData> PlayerDataList;

struct NpcData
{
    uint32 ID = 0;
    uintptr_t EntityAddress = 0;
};
std::vector<NpcData> NpcDataList;

uint64 thisPlayerChatTimer = 0;

#include "native/Natives.h"
#include "FunctionThreadDelays.h"

bool joinedServer = false;
Vector sentPos;
std::string character;

class Witcher3MPClient : public Networking::client_interface<MessageTypes>
{
public:

    void UpdateMessageHandler()
    {
        if (IsConnected())
        {
            while (!Incoming().empty())
            {
                auto msg = Incoming().pop_front().msg;

                switch (msg.header.id)
                {
                    case MessageTypes::TC_REQUEST_PLAYERDATA:
                    {
                        Networking::message<MessageTypes> msg;
                        msg.header.id = MessageTypes::TS_SEND_PLAYERDATA;

                        Vector playerPos;
                        Natives::CActor::GetPositionOrMoveDestination(Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()), &playerPos);

                        uint8 characterId = 1;
                        if (character == "Lambert")
                            characterId = 2;
                        else
                            if (character == "Vesemir")
                                characterId = 3;
                            else
                                if (character == "Zoltan")
                                    characterId = 4;
                                else
                                    if (character == "Letho")
                                        characterId = 5;
                                    else
                                        if (character == "Roche")
                                            characterId = 6;
                                        else
                                            if (character == "Geralt")
                                                characterId = 7;

                        msg << playerPos << characterId;

                        MessageServer(msg);

                        sentPos = playerPos;

                        if (characterId != 7)
                        {
                            std::string command = "SwitchCharacterTo(" + std::to_string(characterId) + ")";
                            FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });
                        }

                        joinedServer = true;

                        if (!FunctionThreadDelayer::func_ExecGameConsoleCommand_locked)
                        {
                            std::string command = "ShowOneLinerByGuidHash(" + std::to_string(Natives::CEntity::GetGuidHash(Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))) + ",\"" + "Connection was sucessful" + "\")";
                            FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });
                            thisPlayerChatTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                        }
                        break;
                    }
                    case MessageTypes::TC_CREATE_PLAYER:
                    {
                        uint32 ID;
                        Vector pos;
                        uint8 characterId;
                        msg >> characterId >> pos >> ID;

                        std::u16string ResName;
                        bool isDepotPath = false;
                        switch (characterId)
                        {
                            case 1:
                            {
                                ResName = u"characters\\npc_entities\\main_npc\\eskel.w2ent";
                                isDepotPath = true;
                                break;
                            }
                            case 2:
                            {
                                ResName = u"characters\\npc_entities\\main_npc\\lambert.w2ent";
                                isDepotPath = true;
                                break;
                            }
                            case 3:
                            {
                                ResName = u"vesemir";
                                isDepotPath = false;
                                break;
                            }
                            case 4:
                            {
                                ResName = u"zoltan";
                                isDepotPath = false;
                                break;
                            }
                            case 5:
                            {
                                ResName = u"characters\\npc_entities\\secondary_npc\\letho.w2ent";
                                isDepotPath = true;
                                break;
                            }
                            case 6:
                            {
                                ResName = u"characters\\npc_entities\\main_npc\\roche.w2ent";
                                isDepotPath = true;
                                break;
                            }
                            case 7:
                            {
                                ResName = u"characters\\npc_entities\\main_npc\\geralt_npc.w2ent";
                                isDepotPath = true;
                                break;
                            }
                        }

                        FunctionThreadDelayer::func_SpawnPlayer_delays.push_back({ ID, ResName, pos,  isDepotPath });
                        break;
                    }
                    case MessageTypes::TC_MASS_CREATE_PLAYER:
                    {
                        for (uint32 i = 0; i < (msg.size() / 20); ++i)
                        {
                            uint32 ID;
                            Vector pos;
                            uint8 characterId;
                            msg >> characterId >> pos >> ID;

                            std::u16string ResName;
                            bool isDepotPath = false;
                            switch (characterId)
                            {
                                case 1:
                                {
                                    ResName = u"characters\\npc_entities\\main_npc\\eskel.w2ent";
                                    isDepotPath = true;
                                    break;
                                }
                                case 2:
                                {
                                    ResName = u"characters\\npc_entities\\main_npc\\lambert.w2ent";
                                    isDepotPath = true;
                                    break;
                                }
                                case 3:
                                {
                                    ResName = u"vesemir";
                                    isDepotPath = false;
                                    break;
                                }
                                case 4:
                                {
                                    ResName = u"zoltan";
                                    isDepotPath = false;
                                    break;
                                }
                                case 5:
                                {
                                    ResName = u"characters\\npc_entities\\secondary_npc\\letho.w2ent";
                                    isDepotPath = true;
                                    break;
                                }
                                case 6:
                                {
                                    ResName = u"characters\\npc_entities\\main_npc\\roche.w2ent";
                                    isDepotPath = true;
                                    break;
                                }
                                case 7:
                                {
                                    ResName = u"characters\\npc_entities\\main_npc\\geralt_npc.w2ent";
                                    isDepotPath = true;
                                    break;
                                }
                            }

                            FunctionThreadDelayer::func_SpawnPlayer_delays.push_back({ ID, ResName, pos, isDepotPath });
                        }
                        break;
                    }
                    case MessageTypes::TC_UPDATE_POS:
                    {
                        uint32 recID;
                        Vector pos;
                        uint8 moveType;
                        msg >> moveType >> pos >> recID;

                        for (auto &i : PlayerDataList)
                        {
                            if (i.ID == recID)
                            {
                                if (pos.null())
                                {
                                    EulerAngles rotation{ 1, 1, 1 };
                                    Natives::CActor::Teleport(i.EntityAddress, &pos, &rotation);
                                    break;
                                }

                                Vector currPos;
                                Natives::CActor::GetPositionOrMoveDestination(i.EntityAddress, &currPos);
                                if (!currPos.null() && (currPos.DistanceTo(pos) > 25.f))
                                {
                                    EulerAngles rotation{ 1, 1, 1 };
                                    Natives::CActor::Teleport(i.EntityAddress, &pos, &rotation);
                                }
                                else
                                {
                                    Natives::CActor::ActionCancelAll(i.EntityAddress);
                                    Natives::CActor::ActionMoveTo(i.EntityAddress, &pos, moveType, 1.f, 1.f, 0, 0, 0);
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case MessageTypes::TC_CREATE_NPC:
                    {
                        uint32 recID;
                        uint32 recResID;
                        Vector recPos;
                        float recHealth;

                        msg >> recHealth >> recPos >> recResID >> recID;

                        std::string ResName = "";
                        ResName = Database::ResourceNames::ResNames[recResID];

                        if (ResName != "")
                        {
                            std::u16string ResName16 = Database::ResourceNames::ToU16(ResName);

                            const char16_t* ResName16char = ResName16.c_str();

                            FunctionThreadDelayer::func_SpawnNpc snpc;
                            snpc.ID = recID;
                            std::copy(ResName16char, ResName16char + 10, snpc.ResName);
                            snpc.pos = recPos;
                            snpc.health = recHealth;

                            FunctionThreadDelayer::func_SpawnNpc_delays.push_back(snpc);
                        }
                        break;
                    }
                    case MessageTypes::TC_SET_ACTOR_HEALTH:
                    {
                        uint32 setHealth_id;
                        bool setHealth_isPlayer;
                        float setHealth_currhealthvalue;
                        float setHealth_maxhealthvalue;

                        msg >> setHealth_maxhealthvalue >> setHealth_currhealthvalue >> setHealth_isPlayer >> setHealth_id;

                        if (setHealth_isPlayer)
                        {
                            if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                            {
                                if (uintptr_t W3AbilityManager = Natives::CActor::GetW3AbilityManager(Player))
                                {
                                    Natives::W3AbilityManager::SetStatPointMax(W3AbilityManager, EBaseCharacterStats::BCS_Vitality, setHealth_maxhealthvalue);
                                    Natives::W3AbilityManager::SetStatPointCurrent(W3AbilityManager, EBaseCharacterStats::BCS_Vitality, setHealth_currhealthvalue);
                                }
                            }
                        }
                        else
                        {
                            for (uint8 i = 0; i < NpcDataList.size(); ++i)
                            {
                                if (NpcDataList[i].ID == setHealth_id)
                                {
                                    NpcData Npcdata = NpcDataList[i];
                                    if (uintptr_t W3AbilityManager = Natives::CActor::GetW3AbilityManager(Npcdata.EntityAddress))
                                    {
                                        if (Natives::W3AbilityManager::GetStatIndex(W3AbilityManager, EBaseCharacterStats::BCS_Vitality) != -1)
                                        {
                                            Natives::W3AbilityManager::SetStatPointMax(W3AbilityManager, EBaseCharacterStats::BCS_Vitality, setHealth_maxhealthvalue);
                                            Natives::W3AbilityManager::SetStatPointCurrent(W3AbilityManager, EBaseCharacterStats::BCS_Vitality, setHealth_currhealthvalue);
                                        }

                                        if (Natives::W3AbilityManager::GetStatIndex(W3AbilityManager, EBaseCharacterStats::BCS_Essence) != -1)
                                        {
                                            Natives::W3AbilityManager::SetStatPointMax(W3AbilityManager, EBaseCharacterStats::BCS_Essence, setHealth_maxhealthvalue);
                                            Natives::W3AbilityManager::SetStatPointCurrent(W3AbilityManager, EBaseCharacterStats::BCS_Essence, setHealth_currhealthvalue);
                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        break;
                    }
                    case MessageTypes::TC_NPC_DEAD:
                    {
                        uint32 recid = 0;
                        msg >> recid;

                        for (auto i : NpcDataList)
                        {
                            if (i.ID == recid)
                            {
                                uint32 targetGuidHash = Natives::CEntity::GetGuidHash(i.EntityAddress);
                                std::string command = "KillActorByGuidHash(" + std::to_string(targetGuidHash) + ")";
                                FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });
                                break;
                            }
                        }
                        break;
                    }
                    case MessageTypes::TC_PLAYER_DEAD:
                    {
                        if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                        {
                            if (uintptr_t W3AbilityManager = Natives::CActor::GetW3AbilityManager(Player))
                            {
                                float maxhealth = Natives::W3AbilityManager::GetStatMax(W3AbilityManager, BCS_Vitality);
                                Natives::W3AbilityManager::SetStatPointCurrent(W3AbilityManager, EBaseCharacterStats::BCS_Vitality, maxhealth);

                                Vector currPos;
                                Natives::CActor::GetPositionOrMoveDestination(Player, &currPos);
                                currPos.z += 500.f;
                                EulerAngles rotation{ 1,1,1 };
                                Natives::CActor::Teleport(Player, &currPos, &rotation);
                            }
                        }
                        break;
                    }
                    case MessageTypes::TC_CHAT_MESSAGE:
                    {
                        if (msg.size() < 4)
                            break;

                        uint32 playerId;

                        size_t i = msg.body.size() - sizeof(uint32);
                        std::memcpy(&playerId, msg.body.data(), sizeof(uint32));
                        msg.body.erase(msg.body.begin(), msg.body.begin() + 4);
                        msg.body.resize(i);
                        msg.header.size = msg.size();

                        if (playerId != 0)
                        {
                            for (PlayerData& i : PlayerDataList)
                            {
                                if (i.ID == playerId)
                                {
                                    std::string chat_message;
                                    for (uint8 i = 0; i < 100; ++i)
                                    {
                                        if (msg.size())
                                        {
                                            char ch;
                                            msg >> ch;
                                            chat_message += ch;
                                        }
                                    }

                                    if (chat_message.length() == 0)
                                        break;

                                    size_t length = chat_message.length();
                                    for (size_t i = 0; i < length / 2; i++)
                                        std::swap(chat_message[i], chat_message[length - i - 1]);

                                    if (!FunctionThreadDelayer::func_ExecGameConsoleCommand_locked)
                                    {
                                        std::string command = "ShowOneLinerByGuidHash(" + std::to_string(Natives::CEntity::GetGuidHash(i.EntityAddress)) + ",\"" + chat_message + "\")";
                                        FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });
                                        i.chatTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                    }
                                    break;
                                }
                            }
                        }
                        else
                        {
                            if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                            {
                                std::string chat_message;
                                for (uint8 i = 0; i < 100; ++i)
                                {
                                    if (msg.size())
                                    {
                                        char ch;
                                        msg >> ch;
                                        chat_message += ch;
                                    }
                                }

                                if (chat_message.length() == 0)
                                    break;

                                size_t length = chat_message.length();
                                for (size_t i = 0; i < length / 2; i++)
                                    std::swap(chat_message[i], chat_message[length - i - 1]);

                                if (!FunctionThreadDelayer::func_ExecGameConsoleCommand_locked)
                                {
                                    std::string command = "ShowOneLinerByGuidHash(" + std::to_string(Natives::CEntity::GetGuidHash(Player)) + ",\"" + chat_message + "\")";
                                    FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });
                                    thisPlayerChatTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
        else
        {
            std::cout << "Server Down\n";
        }
    }
};

Witcher3MPClient* w3client;

#include "native/Hooks.h"

bool canspawn = true;

char modulePath[MAX_PATH];

DWORD APIENTRY MainThread(LPVOID param)
{
    Sleep(10000);

    std::string modulePathstr = modulePath;
    modulePathstr = modulePathstr.substr(0, modulePathstr.find("x64") + 4);
    modulePathstr.append("WitcherSync_Connect.cfg");

    std::ifstream portfile(modulePathstr, std::ifstream::in);
    std::string ipandport;
    std::getline(portfile, ipandport);
    std::getline(portfile, character);
    std::string ipstr = ipandport.substr(0, ipandport.find(':'));
    std::string portstr = ipandport.substr(ipandport.find(':') + 1);

    uint16 port = std::atoi(portstr.c_str());

    w3client = new Witcher3MPClient();

    InstallHooks();

    // no game pause
    unsigned char buffer[] = { 0xc3, 0x90 };
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)(baseAddress + 0xD08FC0), &buffer[0], 2, 0);

    // for testing: allow multiple instances of game
    unsigned char buffer2[] = { 0x90, 0x90 };
    WriteProcessMemory(GetCurrentProcess(), (LPVOID)(baseAddress + 0x3686), &buffer2[0], 2, 0);

    uintptr_t druwner = 0;
    uint64 savedTime = 0;
    uint32 diff = 0;
    while (true)
    {
        w3client->UpdateMessageHandler();

        savedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        if (canspawn)
        {
            if (GetKeyState(VK_NUMPAD1) & 0x8000)
            {

                canspawn = false;
                if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                {
                    //Natives::W3AbilityManager::SetStatPointCurrent(Natives::CActor::GetW3AbilityManager(Player), BCS_Vitality, 2000.f);

                    //Vector currPos;
                    //Natives::CActor::GetPositionOrMoveDestination(Player, &currPos);
                    //if(!druwner)
                    //    druwner =  NativeHelper::CreateEntitySyncByResourceName(u"drowner", currPos, false);
                    //else 
                    //{ 
                    //    Natives::W3AbilityManager::SetStatPointMax(Natives::CActor::GetW3AbilityManager(druwner), BCS_Essence, 6000.f); 
                    //    Natives::W3AbilityManager::SetStatPointCurrent(Natives::CActor::GetW3AbilityManager(druwner), BCS_Essence, 6000.f); 

                    //    uint32 hash = Natives::CEntity::GetGuidHash(druwner);
                    //    druwner = 0; 
                    //}
                }
            }
        }

        if (GetKeyState(VK_NUMPAD2) & 0x8000)
        {
            canspawn = true;
        }

        if (!w3client->IsConnected())
            if (GetKeyState(VK_NUMPAD5) & 0x8000)
                w3client->Connect(ipstr, port);

        if (joinedServer)
        {
            Vector currPos;

            if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                Natives::CActor::GetPositionOrMoveDestination(Player, &currPos);

            if ((!currPos.null())) // GetPositionOrMoveDestination can fail to pass axis data
            {
                float distance = sentPos.DistanceTo(currPos);
                if (distance >= 0.3f)
                {
                    uint8 moveType = MT_WALK;

                    if (distance >= 0.5f)
                        moveType = MT_RUN;
                    else moveType = MT_Sprint;

                    Networking::message<MessageTypes> Updatemsg;
                    Updatemsg.header.id = MessageTypes::TS_NOTIFY_PLAYER_POS_CHANGE;

                    Updatemsg << currPos << moveType;
                    w3client->MessageServer(Updatemsg);

                    sentPos = currPos;
                }
            }

            uint64 currTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if (thisPlayerChatTimer != 0)
            {
                if (uintptr_t Player = Natives::CCommonGame::GetPlayerEntity(Natives::GVars::GCommonGame()))
                {
                    if ((currTime - thisPlayerChatTimer) > 4000)
                    {
                        if (!FunctionThreadDelayer::func_ExecGameConsoleCommand_locked)
                        {
                            std::string command = "HideOneLinerByGuidHash(" + std::to_string(Natives::CEntity::GetGuidHash(Player)) + ")";
                            FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });

                            thisPlayerChatTimer = 0;
                        }
                    }
                }
            }

            for (auto& i : PlayerDataList)
            {
                if (i.chatTimer != 0)
                {
                    if ((currTime - i.chatTimer) > 4000)
                    {
                        if (!FunctionThreadDelayer::func_ExecGameConsoleCommand_locked)
                        {
                            std::string command = "HideOneLinerByGuidHash(" + std::to_string(Natives::CEntity::GetGuidHash(i.EntityAddress)) + ")";
                            FunctionThreadDelayer::func_ExecGameConsoleCommand_delays.push_back({ command });

                            i.chatTimer = 0;
                        }
                    }
                }
            }
        }

        diff = (uint32)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - savedTime);
    }

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    GetModuleFileNameExA(GetCurrentProcess(), hModule, modulePath, sizeof(modulePath));

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

