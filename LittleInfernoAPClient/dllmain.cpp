// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <Psapi.h>
#include <climits>
#include "checkhandler.h"

using namespace std;
DWORD ModuleBase = 0;
bool isRunning = false;

bool InitGetModuleHandle() {
    DWORD pid = GetCurrentProcessId();
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    HMODULE hModules[1024];
    DWORD cbNeeded;
    if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
        std::cerr << "Failed to enumerate modules." << std::endl;
        CloseHandle(hProcess);
        return 0;
    }
    ModuleBase = (DWORD)hModules[0];
    return 1;
}


DWORD WINAPI RunGameLoop(LPVOID lpParam) {
    // GameHandler::SetupOnInit();
    checkHandler::setupHooks(ModuleBase);
    isRunning = true;
    while (isRunning) {
        int saveSlot = *(int*)(ModuleBase + 0x0761FD8);
        if (saveSlot != -1) {
            int saveDataPointer = *(int*)(ModuleBase + 0x761FD4);
            int saveDataAddress = saveDataPointer + saveSlot * 0x428C;
            int coinCount = *(int*)(saveDataAddress + 0xC);
            *(int*)(saveDataAddress + 0xC) = 999;
            int* stampCountPointer = (int*)(saveDataAddress + 0x10);
            *stampCountPointer = 99;
        }
    }
    return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!InitGetModuleHandle())
            return FALSE;
        QueueUserWorkItem(RunGameLoop, NULL, WT_EXECUTEDEFAULT);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void Init()
{
    if (!InitGetModuleHandle())
        return;
    RunGameLoop(NULL);
}


