#pragma once

#include <cstddef>

#include "../Pad.h"

class GlobalVars {
public:
    float realTime; //0x0000 
    int frameCount; //0x0004 
    float unknown1; //0x0008 
    float unknown2; //0x000C 
    int maxClients; //0x0010 
    float intervalPerTick; //0x0014 
    int unknown3; //0x0018 
    int unknown4; //0x001C 
    void* m_unkfunc; //0x0020 
    float unknown5; //0x0028 
    float currentTime; //0x002C 
    float currentTime2; //0x0030 
    PAD(0xC) //0x0034
    int tickCount; //0x0040 
    float intervalPerTick2; //0x0044 
    void* currentNetchan; //0x0048 
    PAD(0x130) //0x0050
    char* currentMap; //0x0180 
    char* currentMapname; //0x0188 
}; //Size=0x0190
