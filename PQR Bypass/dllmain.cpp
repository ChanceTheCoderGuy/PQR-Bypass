// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <string>
#include <sstream>
#include "MemoryOperation.h"
#include "Detour.h"
#include "Detours.h"


DWORD MainThreadControl(LPVOID lpParm);
void FixInvaildPointers();
void GetDevicePointers();
void CleanUp();
int endScenePointer{};
int resetPointer{};
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  Reason, LPVOID lpReserved)
{
    switch (Reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainThreadControl, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        CleanUp();
        break;
    }
    return TRUE;
}
DWORD MainThreadControl(LPVOID lpParm)
{
    FixInvaildPointers();
    GetDevicePointers();
    _Detours["FrameScriptExecute"] = new Detour(0x00819210, (int)FrameScriptExecuteDetour);
    return 0;
}


void CleanUp()
{
    for(auto obj : _Detours)
        delete obj.second;

    _Detours.clear();
}


void FixInvaildPointers()
{
    *(int*)0x00D415B8 = 1;
    *(int*)0x00D415BC = 0x7FFFFFFF;
}

void GetDevicePointers()
{
    // Device
    int ptr;
    {
        ptr = *(int*)0x00C5DF88;
    }while (ptr == 0);

    {
        ptr = *(int*)(ptr + 0x397C);
    }while (ptr == 0);

    // Scene	
    {
        ptr = *(int*)ptr;
    }while (ptr == 0);

    {
        endScenePointer = *(int*)(ptr + 0xA8);   //42 * 4	
    }while (endScenePointer == 0);

    {
        resetPointer = *(int*)(ptr + 0x40);   //16 * 4	
    }while (resetPointer == 0);
}

