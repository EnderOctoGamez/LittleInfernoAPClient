#pragma once
#include "pch.h"
#include "minhook.h"
#include <iostream>

class checkHandler {
public :
	static void setupHooks(DWORD ModuleBase);
	static void __stdcall getComboHook();
	static void sendCombo(int comboID);
};