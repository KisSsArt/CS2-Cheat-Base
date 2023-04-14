#pragma once

#include "../Pad.h"
#include "../Vector.h"

class CInButtonStatePB {
public:
    PAD(24) //0x0000
    uint32_t buttons; //0x0018
    PAD(12) //0x001C
}; //Size: 0x0030

class CmdQAngle {
public:
    PAD(24) //0x0000
    Vector angles;
    PAD(1052) //0x0024
}; //Size: 0x0440

class UserCmdBase {
public:
    PAD(56) //0x0000
    CInButtonStatePB* button_state; //0x0038
    CmdQAngle* viewangles; //0x0040
    float unk1; //0x0048
    float unk2; //0x004C
    float forwardmove; //0x0050
    float sidemove; //0x0054
    float unk3; //0x0058
    float unk4; //0x005C
    float unk5; //0x0060
    float unk6; //0x0064
    float unk7; //0x0068
    PAD(12) //0x006C
}; //Size: 0x0440

class CMsgQAngle {
public:
    PAD(24) // 0x0000
    Vector angles;
    PAD(40) // 0x0020
};                      // Size: 0x0050

struct SubTickCmd
{
    PAD(0x18);
    CMsgQAngle* AnglesMsg;
};

struct SubTicksContainer
{
    uint32_t count;
    PAD(0x4);
    uint64_t memory;

    SubTickCmd* Get(int i)
    {
        if (i < count)
        {
            SubTickCmd** list = (SubTickCmd**)(memory + 0x8);
            return list[i];
        }
        return nullptr;
    }
};

struct UserCmd {
public:
    enum {
        IN_ATTACK = 1 << 0,
        IN_JUMP = 1 << 1,
        IN_DUCK = 1 << 2,
        IN_FORWARD = 1 << 3,
        IN_BACK = 1 << 4,
        IN_USE = 1 << 5,
        IN_TURNLEFT = 1 << 7,
        IN_TURNRIGHT = 1 << 8,
        IN_MOVELEFT = 1 << 9,
        IN_MOVERIGHT = 1 << 10,
        IN_ATTACK2 = 1 << 11,
        IN_RELOAD = 1 << 13,
        IN_SPEED = 1 << 16,
        IN_JOYAUTOSPRINT = 1 << 17,
        IN_FIRST_MOD_SPECIFIC_BIT = 1 << 32,
        IN_USEORRELOAD = 1 << 32,
        IN_SCORE = 8589934592,
        IN_ZOOM = 17179869184,
        IN_JUMP_THROW_RELEASE = 34359738368,
    };

    PAD(48);               //0x0000
    UserCmdBase* base; //0x0030
    PAD(24);               //0x0038
    uint32_t buttons;      //0x0050
    PAD(1004);             //0x0054

    UserCmdBase* getBase()
    {
        return base;
    }

    SubTicksContainer GetSubTicksContainer()
    {
        return *(SubTicksContainer*)((uintptr_t)this + 0x20);
    }

    void setSubTickAngles(const Vector& Angles)
    {
        auto subTicks = GetSubTicksContainer();
        for (int i = 0; i < subTicks.count; i++)
        {
            auto* tick = subTicks.Get(i);
            if (tick && tick->AnglesMsg)
                tick->AnglesMsg->angles = Angles;
        }
    }
};
