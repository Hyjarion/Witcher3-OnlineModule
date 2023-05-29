#include "NativeTypes.h"

namespace Natives
{
    // global variables
    namespace GVars
    {
        uintptr_t GGame() { return *(reinterpret_cast<uintptr_t*>(baseAddress + 0x2AA0A70)); }
        uintptr_t GDepot() { return *(reinterpret_cast<uintptr_t*>(baseAddress + 0x297D358)); }
        uintptr_t GCommonGame() { return *(reinterpret_cast<uintptr_t*>(baseAddress + 0x2B36CD8)); }
    }

    namespace W3AbilityManager
    {
        // 0xE3A900
        int32 GetStatIndex(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats)
        {
            typedef int32(*_GetStatIndex)(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats);
            _GetStatIndex GetStatIndex = (_GetStatIndex)(baseAddress + 0xE3A900);
            return GetStatIndex(W3AbilityManager, EBaseCharacterStats);
        }

        float GetStatPointCurrent(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats)
        {
            int64 StatIndex = GetStatIndex(W3AbilityManager, EBaseCharacterStats);
            if (StatIndex == -1)
                return -1.f;
            else
            {
                int64 index = (StatIndex + StatIndex * 2);
                uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(W3AbilityManager + 0x60));
                return *(reinterpret_cast<float*>(a1 + 4 * index));
            }
        }

        void SetStatPointCurrent(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats, float value)
        {
            int64 StatIndex = W3AbilityManager::GetStatIndex(W3AbilityManager, EBaseCharacterStats);
            if (StatIndex != -1)
            {
                int64 index = (StatIndex + StatIndex * 2);
                uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(W3AbilityManager + 0x60));
                *(reinterpret_cast<float*>(a1 + 4 * index)) = value;
            }
        }

        float GetStatMax(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats)
        {
            int64 StatIndex = GetStatIndex(W3AbilityManager, EBaseCharacterStats);
            if (StatIndex == -1)
                return -1.f;
            else
            {
                int64 index = (StatIndex + StatIndex * 2);
                uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(W3AbilityManager + 0x60));
                return *(reinterpret_cast<float*>(a1 + 4 * index + 4));
            }
        }

        void SetStatPointMax(uintptr_t W3AbilityManager, uint32 EBaseCharacterStats, float value)
        {
            int64 StatIndex = W3AbilityManager::GetStatIndex(W3AbilityManager, EBaseCharacterStats);
            if (StatIndex != -1)
            {
                int64 index = (StatIndex + StatIndex * 2);
                uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(W3AbilityManager + 0x60));
                *(reinterpret_cast<float*>(a1 + 4 * index + 4)) = value;
            }
        }
    }

    namespace CActor
    {
        // 0x76FD60
        void ActionCancelAll(uintptr_t CActor)
        {
            typedef void(*_ActionCancelAll)(uintptr_t CActor);
            _ActionCancelAll ActionCancelAll = (_ActionCancelAll)(baseAddress + 0x76FD60);
            ActionCancelAll(CActor);
        }

        // 0xAC9CF0
        bool ActionMoveTo(uintptr_t CActor, Vector* pos, uint32 EMoveType, float absSpeed, float radius, uint32 EMoveFailureAction, uint16 unk, float unk2)
        {
            typedef bool(*_ActionMoveTo)(uintptr_t CActor, Vector* pos, uint32 EMoveType, float absSpeed, float radius, uint32 EMoveFailureAction, uint16 unk, float unk2);
            _ActionMoveTo ActionMoveTo = (_ActionMoveTo)(baseAddress + 0xAC9CF0);
            return ActionMoveTo(CActor, pos, EMoveType, absSpeed, radius, EMoveFailureAction, unk, unk2);
        }

        // 0xACC3C0
        bool ActionRotateTo(uintptr_t CActor, Vector* pos, bool unk, float unk2)
        {
            typedef bool (*_ActionRotateTo)(uintptr_t CActor, Vector* pos, bool unk, float unk2);
            _ActionRotateTo ActionRotateTo = (_ActionRotateTo)(baseAddress + 0xACC3C0);
            return ActionRotateTo(CActor, pos, unk, unk2);
        }

        // 0x76F4A0
        bool Teleport(uintptr_t CActor, Vector* pos, EulerAngles* rotation)
        {
            typedef bool(*_Teleport)(uintptr_t CActor, Vector* pos, EulerAngles* rotation);
            _Teleport Teleport = (_Teleport)(baseAddress + 0x76F4A0);
            return Teleport(CActor, pos, rotation);
        }

        uintptr_t GetPositionOrMoveDestination(uintptr_t CActor, Vector* result)
        {
            typedef uintptr_t(*_GetPositionOrMoveDestination)(uintptr_t CActor, Vector* result);
            _GetPositionOrMoveDestination GetPositionOrMoveDestination = (_GetPositionOrMoveDestination)(baseAddress + 0x769580);
            return GetPositionOrMoveDestination(CActor, result);
        }

        uintptr_t GetW3AbilityManager(uintptr_t CActor)
        {
            if(uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(CActor + 0x20)))
                if(uintptr_t a2 = *(reinterpret_cast<uintptr_t*>(a1 + 0x158)))
                    if (uintptr_t a3 = *(reinterpret_cast<uintptr_t*>(a2 + 0x10)))
                        return a3;
            return 0;
        }

        uintptr_t GetTarget(uintptr_t CActor)
        {
            //typedef uintptr_t(*_GetTarget)(uintptr_t CActor);                        // 0x76AA80, not sure what this used for, but not what I need
            //_GetTarget GetTarget = (_GetTarget)(baseAddress + 0x76AA80);
            //return GetTarget(CActor);

            typedef uintptr_t(*_func)(uintptr_t CActor);
            _func func = (_func)(*reinterpret_cast<uintptr_t*>((*(reinterpret_cast<uintptr_t*>(CActor))) + 0x600));

            if (uintptr_t a1 = func(CActor))
                //if (uintptr_t a2 = *(reinterpret_cast<uintptr_t*>(a1 + 0x8)))  // Somewhy this part is included in the native function (0xADC650) and makes it not return as the target's proper class address,
                //    return a2;                                                 // but if called the function from .ws script it returns as intended for some reason, 
                                                                                 // so in here to get the proper address I'll skip this procedure and return with a1 which is the address of the target.
                return a1;

            return 0;
        }
    }

    namespace CEntity
    {
        uint32 GetGuidHash(uintptr_t CEntity)
        {
            uint32 a1 = *(reinterpret_cast<uint32*>(CEntity + 0xD8));
            a1 = a1 ^ (*(reinterpret_cast<uint32*>(CEntity + 0xD4)));
            a1 = a1 ^ (*(reinterpret_cast<uint32*>(CEntity + 0xD0)));
            a1 = a1 ^ (*(reinterpret_cast<uint32*>(CEntity + 0xCC)));
            return a1;
        }
    }

    namespace Clayer
    {
        // 0x17B130
        uintptr_t CreateEntitySync(uintptr_t CLayer, EntitySpawnInfo* entitySpawnInfo)
        {
            typedef uintptr_t(*_CreateEntitySync)(uintptr_t CLayer, EntitySpawnInfo* entitySpawnInfo);
            _CreateEntitySync CreateEntitySync = (_CreateEntitySync)(baseAddress + 0x17B130);
            return CreateEntitySync(CLayer, entitySpawnInfo);
        }

        // 0x17AF80
        uintptr_t CreateEntityAsync(uintptr_t CLayer, EntitySpawnInfo* entitySpawnInfo)
        {
            typedef uintptr_t(*_CreateEntityAsync)(uintptr_t CLayer, EntitySpawnInfo* entitySpawnInfo);
            _CreateEntityAsync CreateEntityAsync = (_CreateEntityAsync)(baseAddress + 0x17AF80);
            return CreateEntityAsync(CLayer, entitySpawnInfo);
        }
    }

    namespace GDepot
    {
        // 0x888A0
        uintptr_t NMLoadResource(uintptr_t GDepot, uintptr_t& result, Tstring* ResourceName, char boolunk)
        {
            typedef uintptr_t(*_NMLoadResource)(uintptr_t GDepot, uintptr_t& result, Tstring* ResourceName, char boolunk);
            _NMLoadResource NMLoadResource = (_NMLoadResource)(baseAddress + 0x888A0);
            return NMLoadResource(GDepot, result, ResourceName, boolunk);
        }
    }

    namespace CGame
    {
        bool IsActive() { return *(reinterpret_cast<bool*>(GVars::GGame() + 0x110)); }
    }

    namespace CCommonGame
    {
        uintptr_t GetCEntityPool(uintptr_t CCommonGame) { return *(reinterpret_cast<uintptr_t*>(CCommonGame + 0x11B30)); }
        uintptr_t GetPlayerEntity(uintptr_t CCommonGame)
        {
            if (uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(CCommonGame + 0x0BA00)))
                return *reinterpret_cast<uintptr_t*>(a1 + 0x10);
            return 0;
        }

        uintptr_t GetCR4GuiManager(uintptr_t CCommonGame)
        {
            uintptr_t result = 0;

            if (uintptr_t a1 = *reinterpret_cast<uintptr_t*>((*reinterpret_cast<uintptr_t*>(CCommonGame)) + 0x1A8))
            {
                typedef uintptr_t(*_func)(uintptr_t CCommonGame);
                _func func = (_func)(a1);
                result = func(CCommonGame);
            }
            return result;
        }

        void ShowOneLiner(uintptr_t CCommonGame, Tstring* text, uintptr_t CEntity) // Works but cannot be hidden including via .ws script
        {
            if (uintptr_t CR4GuiManager = GetCR4GuiManager(CCommonGame))
            {
                typedef void(*_ShowOneLiner)(uintptr_t CR4GuiManager, Tstring* text, uintptr_t CEntity);
                _ShowOneLiner ShowOneLiner = (_ShowOneLiner)(baseAddress + 0xD62480);
                ShowOneLiner(CR4GuiManager, text, CEntity);
            }
        }

        void HideOneLiner(uintptr_t CCommonGame, uintptr_t CEntity) // Doesn't work at all
        {
            if (uintptr_t CR4GuiManager = GetCR4GuiManager(CCommonGame))
            {
                typedef void(*_HideOneLiner)(uintptr_t CR4GuiManager, uintptr_t CEntity);
                _HideOneLiner HideOneLiner = (_HideOneLiner)(baseAddress + 0xD66680);
                HideOneLiner(CR4GuiManager, CEntity);
            }
        }
    }

    namespace CEntityPool
    {
        // 0xAAB0D0
        uintptr_t SpawnEntitySync(uintptr_t CEntityPool, EntitySpawnInfo* entitySpawnInfo)
        {
            typedef uintptr_t(*_SpawnEntitySync)(uintptr_t CEntityPool, EntitySpawnInfo* entitySpawnInfo);
            _SpawnEntitySync SpawnEntitySync = (_SpawnEntitySync)(baseAddress + 0xAAB0D0);
            return SpawnEntitySync(CEntityPool, entitySpawnInfo);
        }
    }

    // 0x2C25B0
    bool CScriptInvoker_Parse(Tstring* command)
    {
        typedef bool(*_CScriptInvoker_Parse)(Tstring*);
        _CScriptInvoker_Parse CScriptInvoker_Parse = (_CScriptInvoker_Parse)(baseAddress + 0x2C25B0);
        return CScriptInvoker_Parse(command);
    }
}

namespace NativeHelper
{
    uintptr_t GetEntityInfo(uintptr_t ResourceAddress)
    {
        if (uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(ResourceAddress + 0x10)))
            if (uintptr_t a2 = *(reinterpret_cast<uintptr_t*>(a1 + 0x8)))
                return a2;
        return 0;
    }

    uintptr_t GetActiveLayer(uintptr_t CGameAddress)
    {
        if (uintptr_t a1 = *(reinterpret_cast<uintptr_t*>(CGameAddress + 0xf0)))
            if (uintptr_t a2 = *(reinterpret_cast<uintptr_t*>(a1 + 0x10)))
                if (uintptr_t a3 = *(reinterpret_cast<uintptr_t*>(a2 + 0x70)))
                    return a3;
        return 0;
    }

    uintptr_t CreateEntitySyncByResourceName(std::u16string ResName, Vector pos, bool isDepotPath)
    {
        std::u16string ResPath;

        if (isDepotPath)
        {
            ResPath = ResName;
        }
        else
        {
            ResPath = u"resdef:";
            ResPath.append(ResName);
        }

        uintptr_t result = 0;
        char16_t* Resname = const_cast<char16_t*>(ResPath.c_str());
        Tstring ResnameTstring{ &Resname[0], (uint32)ResPath.length() + 1 };
        Natives::GDepot::NMLoadResource(Natives::GVars::GDepot(), result, &ResnameTstring, 1);

        EntitySpawnInfo entityspawninfo;
        uintptr_t entityInfo = GetEntityInfo(result);
        entityspawninfo.EntityInfo = entityInfo;
        entityspawninfo.SpawnPosition[0] = pos.x;
        entityspawninfo.SpawnPosition[1] = pos.y;
        entityspawninfo.SpawnPosition[2] = pos.z;
        entityspawninfo.SpawnPosition[3] = pos.o;
        entityspawninfo.unk11 = 2;

        return Natives::Clayer::CreateEntitySync(GetActiveLayer(Natives::GVars::GGame()), &entityspawninfo);
    }

    void ExecGameConsoleCommand(std::string command)
    {
        std::u16string command16 = Database::ResourceNames::ToU16(command);
        char16_t* Resname = const_cast<char16_t*>(command16.c_str());
        Tstring exec{ &Resname[0], (uint32)command16.length() + 1 };
        Natives::CScriptInvoker_Parse(&exec);
    }
}