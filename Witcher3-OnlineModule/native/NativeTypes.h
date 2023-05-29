#pragma pack(push, 1)
struct EntitySpawnInfo
{
    uint8 unk0[12]{};
    uint64 unk1 = 0;
    uint8 unk2[20]{};
    uint64 EntityInfo = 0;
    uint64 unk3 = 0;
    uint64* unk4Info = 0;
    uint64 unk5 = 0;
    uint64 unk6 = 0;
    float SpawnPosition[4]{};
    uint64 unk7 = 0;
    float Rotation = 1.f;
    uint32 unk8 = 0;
    float unk9[4]{ 1.f, 1.f, 1.f, 1.f };
    uint8 unk10[42]{};
    uint16 unk11 = 0;
    int32 unk12 = -1;
};
#pragma pack(pop)

struct Tstring
{
    char16_t* string;
    uint32 size;
};

struct Vector
{
    float x = 0;
    float y = 0;
    float z = 0;
    float o = 1;

    float DistanceTo(Vector pos)
    {
        float d = sqrt(pow(pos.x - x, 2.f) + pow(pos.y - y, 2.f) + pow(pos.z - z, 2.f) * 1.f);
        return d;
    }

    bool null()
    {
        if (x && y && z)
            return false;
        return true;
    }
};

struct EulerAngles
{
    float roll, pitch, yaw;
};

enum EMoveTypes
{
    MT_WALK     = 0,
    MT_RUN      = 1,
    MT_FastRun  = 2,
    MT_Sprint   = 3,
    MT_AbsSpeed = 4,
    MT_Rotate   = 5
};

enum EBaseCharacterStats
{
    BCS_Vitality,
    BCS_Essence,
    BCS_Stamina,
    BCS_Toxicity,
    BCS_Focus,
    BCS_Morale,
    BCS_Air,
    BCS_Panic,
    BCS_PanicStatic,
    BCS_SwimmingStamina,
    BCS_Undefined
};
