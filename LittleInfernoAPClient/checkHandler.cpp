#include "pch.h"
#include "checkHandler.h"

typedef void(__stdcall* FunctionType)();
FunctionType comboHookOrigin = nullptr;

uintptr_t comboHookReturnAddress;
_declspec(naked) void __stdcall checkHandler::getComboHook() {
	_asm {
		pushad
		push esi
		call checkHandler::sendCombo
		add esp,4
		popad
		add edx,ecx
		or ecx,-01
		jmp dword ptr[comboHookReturnAddress]
	}
}
void checkHandler::sendCombo(int comboID) {
	std::cout << comboID << std::endl;
}

void checkHandler::setupHooks(DWORD ModuleBase){
	MH_Uninitialize();
	MH_Initialize();

	auto comboHookAddress = (char*)ModuleBase + 0x914D7;
	comboHookReturnAddress = ModuleBase + 0x914D7 + 5;
	MH_CreateHook((LPVOID)comboHookAddress, &getComboHook, reinterpret_cast<LPVOID*>(&comboHookOrigin));
	MH_EnableHook(MH_ALL_HOOKS);
}

