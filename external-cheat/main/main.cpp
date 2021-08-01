#include "ui.h"
using namespace std;

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"
#include <direct.h>
#include <tchar.h>
#include <vector>

#include "..\cheat\xor.h"
#include "..\cheat\driver.h"

#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

UINT64 Gbase;
UINT64 Ubase;
HANDLE memory_read = NULL, memory_write = NULL, memory_esp_write = NULL;

void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void InitializeDriver() {
    if (!driver::open_memory_handles());

    std::cout << _xor_("Open Rust, join server and press enter.") << std::endl;
    cin.get();
    HideConsole();

    if (driver::get_process_id(_xor_("RustClient.exe").c_str()));

    Gbase = (UINT64)driver::get_module_base_address(_xor_("GameAssembly.dll").c_str());
    Ubase = (UINT64)driver::get_module_base_address(_xor_("UnityPlayer.dll").c_str());
}

int main(int argc, char** argv)
{
    LoadLibraryA("WINMM.dll");
    m_render = new render_base(NULL() + "script.cfg");
    m_render->Game_Loop();
    return 0;
}