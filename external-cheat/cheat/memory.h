#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include "driver.h"

static HANDLE hProcess;
extern UINT64 Gbase;
extern UINT64 Ubase;

static uintptr_t ReadChain(uintptr_t base, const std::vector<uintptr_t>& offsets) {
	uintptr_t result = driver::read<uintptr_t>(base + offsets.at(0));
	for (int i = 1; i < offsets.size(); i++) {
		result = driver::read<uintptr_t>(result + offsets.at(i));
	}
	return result;
}