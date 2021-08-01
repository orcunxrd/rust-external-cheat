#include <Windows.h>
#include <iostream>
#pragma comment(lib, "winmm.lib")

#include "..\cheat\cheat.h"
#include "..\cheat\vector.h"
#include "..\mainrenderer.h"
#include "..\vendor\DirectX9\Include\D3DX10math.h"

using namespace std;
using namespace ImGui;

render_base* m_render = nullptr;
render_base::render_base(std::string local_cfg) {
	srand(time(nullptr));
}

void render_base::Game_Loop() {
	RECT r;
	HWND hWnd = GetConsoleWindow();
	HWND console = GetConsoleWindow();
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	SetLayeredWindowAttributes(hWnd, 0, 500, LWA_ALPHA);
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 600, 399, TRUE);
	WinMain(0, 0, 0, 0);
}

int StartingThreads;
UINT64 oPlayerList;
bool Initialized;

void Initialize() {
	UpdateLocalPlayer();
	oPlayerList = 0;
	if (!oPlayerList) {
		UINT64 val = driver::read<UINT64>(Gbase + oBaseNetworkable);
		UINT64 st = driver::read<UINT64>(val + 0xB8);
		UINT64 listptr = driver::read<UINT64>(st + 0x0);
		UINT64 list = driver::read<UINT64>(listptr + 0x10);
		oPlayerList = driver::read<UINT64>(list + 0x28);
	}

	client_ents = oPlayerList;
	Initialized = true;
}

int menus = 0;

void render_base::Draw_Render_Menu(IDirect3DDevice9* pDevice)
{
	ImGuiIO& io = ImGui::GetIO();

	if (!Initialized) {
		//Initialize();
	/*	std::thread(EntityUpdate).detach();
		std::thread(EntityDiscovery).detach();*/
		StartingThreads++;
	}

	ImGui_ImplDX9_NewFrame();
	int CenterX = GetSystemMetrics(0) / 2 - 1;
	int CenterY = GetSystemMetrics(1) / 2 - 1;
	if (Globals::bShowMenu) // menu bool
	{
		ImGui::StyleColorsDark();

		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);

		float x = w * 0.5f - 300.0f;
		float y = h * 0.5f - 200.0f;

		ImGui::SetNextWindowPos(ImVec2{ x, y }, ImGuiCond_Once);
		ImGui::SetNextWindowSize(ImVec2{ 600, 270 }, ImGuiCond_Once);
		ImGuiIO& io = ImGui::GetIO();

		ImGui::Begin("", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		{		
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.f, 1.f, 1.f, 1.f));
			
			ImGui::BeginGroup();
			{
				if (ImGui::Button((_xor_("Aimbot").c_str()), ImVec2(70, 34))) menus = 0;
				ImGui::SameLine();
				if (ImGui::Button((_xor_("Visuals").c_str()), ImVec2(70, 34))) menus = 1;
				ImGui::SameLine();
				if (ImGui::Button((_xor_("Misc").c_str()), ImVec2(70, 34))) menus = 2;

				ImGui::PopStyleColor();
			}
			ImGui::EndGroup();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);

			if (menus == 0) {

				ImGui::SetWindowSize(ImVec2{ 238, 210 });
				ImGui::BeginGroup();
				{
					ImGui::Text(_xor_("Aim Features:").c_str());
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);

					ImGui::Checkbox(_xor_("##AimbotOn").c_str(), &Vars::Aim::AimBot);
					ImGui::SameLine(); ImGui::Text(_xor_("Aimbot (Capslock)").c_str());

					ImGui::Checkbox(_xor_("##Prediction").c_str(), &Vars::Aim::Prediction);
					ImGui::SameLine(); ImGui::Text(_xor_("Prediction").c_str());

					ImGui::Checkbox(_xor_("##Silent").c_str(), &Vars::Aim::SilentAim);
					ImGui::SameLine(); ImGui::Text(_xor_("Silent Aim").c_str());

					ImGui::Checkbox(_xor_("##DrawFov").c_str(), &Vars::Visuals::DrawFov);
					ImGui::SameLine(); ImGui::Text(_xor_("DrawFov").c_str());
					ImGui::SameLine(95); ImGui::ColorEdit4(_xor_("DrawFovColor##3").c_str(), (float*)&Vars::Aim::DrawFovColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					if (Vars::Visuals::DrawFov) {
						ImGui::SliderInt(_xor_("##DrawFovValue").c_str(), &Vars::Aim::DrawFovValue, 0, 350);
					}

					ImGui::Checkbox("##Crosshair", &Vars::Aim::Crosshair);
					ImGui::SameLine(); ImGui::Text("Crosshair");
					ImGui::SameLine(95); ImGui::ColorEdit4("CrosshairColor##3", (float*)&Vars::Aim::DrawCrosshairColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

					if (ImGui::Button(_xor_("Exit").c_str(), ImVec2(50, 0))) exit(0);
				}
				ImGui::EndGroup();
			}
			if (menus == 1) {
				ImGui::SetWindowSize(ImVec2{ 500, 312 });
				ImGui::BeginGroup();
				{
					ImGui::Columns(2);
					{
						ImGui::Text(_xor_("Player Features:").c_str());
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);

						ImGui::Checkbox(_xor_("##PlayerESP").c_str(), &Vars::Visuals::Players::PlayerESP);
						ImGui::SameLine(); ImGui::Text(_xor_("Player ESP").c_str());

						ImGui::SliderInt(_xor_("##ESPRange").c_str(), &Vars::Visuals::Players::PlayerESPRange, 10, 280); ImGui::SameLine(); ImGui::Text(_xor_("Range").c_str());

						if (Vars::Visuals::Players::PlayerESP) {

							ImGui::BeginGroup();
							{
								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Player:").c_str());

									ImGui::Checkbox(_xor_("##NameESP").c_str(), &Vars::Visuals::Players::DisplayName);
									ImGui::SameLine(); ImGui::Text(_xor_("Distance + Name").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("NameESPColor##3").c_str(), (float*)&Vars::Visuals::Players::NameEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##BeltItems").c_str(), &Vars::Visuals::BeltItemsBool);
									ImGui::SameLine(); ImGui::Text(_xor_("Belt Items").c_str());

									ImGui::Checkbox(_xor_("##HpBar").c_str(), &Vars::Visuals::Players::HP);
									ImGui::SameLine(); ImGui::Text(_xor_("HP Bar").c_str());
									ImGui::SameLine(85); ImGui::ColorEdit4(_xor_("HpBarColor##3").c_str(), (float*)&Vars::Visuals::Players::InHpBarColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
									ImGui::SameLine(100); ImGui::ColorEdit4(_xor_("HpBarColor##4").c_str(), (float*)&Vars::Visuals::Players::OutHpBarColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();

								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);

								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Box:").c_str());

									ImGui::Checkbox(_xor_("##BoxESP").c_str(), &Vars::Visuals::Players::Box);
									ImGui::SameLine(); ImGui::Text(_xor_("Box ESP").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("BoxEspColor##3").c_str(), (float*)&Vars::Visuals::Players::BoxEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##SleepESP").c_str(), &Vars::Visuals::Players::IgnoreSleeping);
									ImGui::SameLine(); ImGui::Text(_xor_("Sleep").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("SleepEspColor##3").c_str(), (float*)&Vars::Visuals::Players::SleepEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##FilledESP").c_str(), &Vars::Visuals::Players::FilledBox);
									ImGui::SameLine(); ImGui::Text(_xor_("Filled").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("FilledEspColor##3").c_str(), (float*)&Vars::Visuals::Players::FilledEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##ActiveESP").c_str(), &Vars::Visuals::Players::ActiveESP);
									ImGui::SameLine(); ImGui::Text(_xor_("Active").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("ActiveEspColor##3").c_str(), (float*)&Vars::Visuals::Players::ActiveEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##SkeletonESP").c_str(), &Vars::Visuals::Players::Skeleton);
									ImGui::SameLine(); ImGui::Text(_xor_("Skeleton").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("SkeletonEspColor##3").c_str(), (float*)&Vars::Visuals::Players::SkeletonEspColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();
							}
							ImGui::EndGroup();
						}
					}
					ImGui::NextColumn();
					{
						ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
						ImGui::BeginGroup();
						{
							ImGui::Text(_xor_("Other Features:").c_str());
							ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);

							ImGui::Checkbox(_xor_("##PrefabESP").c_str(), &Vars::Visuals::PrefabEsp);
							ImGui::SameLine(); ImGui::Text(_xor_("Prefab ESP").c_str());

							if (Vars::Visuals::PrefabEsp) {

								ImGui::SliderInt(_xor_("##PrefabRange").c_str(), &Vars::Visuals::PrefabEspRange, 10, 280); ImGui::SameLine(); ImGui::Text(_xor_("Range").c_str());

								ImGui::Checkbox(_xor_("##ShowDistance").c_str(), &Vars::Visuals::WORLD::ShowDistance);
								ImGui::SameLine(); ImGui::Text(_xor_("Show Distance").c_str());

								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Ores:").c_str());

									ImGui::Checkbox(_xor_("##SulfurESP").c_str(), &Vars::Visuals::WORLD::Ores::Sulfur);
									ImGui::SameLine(); ImGui::Text(_xor_("Sulfur").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("SulfurColor##3").c_str(), (float*)&Vars::Visuals::WORLD::Ores::SulfurColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##StoneESP").c_str(), &Vars::Visuals::WORLD::Ores::Stone);
									ImGui::SameLine(); ImGui::Text(_xor_("Stone").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("StoneColor##3").c_str(), (float*)&Vars::Visuals::WORLD::Ores::StoneColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##MetalESP").c_str(), &Vars::Visuals::WORLD::Ores::Metal);
									ImGui::SameLine(); ImGui::Text(_xor_("Metal").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("MetalColor##3").c_str(), (float*)&Vars::Visuals::WORLD::Ores::MetalColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();

								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);

								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Collectables:").c_str());

									ImGui::Checkbox(_xor_("##WoodESP").c_str(), &Vars::Visuals::WORLD::Wood);
									ImGui::SameLine(); ImGui::Text(_xor_("Wood").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("WoodColor##3").c_str(), (float*)&Vars::Visuals::WORLD::WoodColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##HempESP").c_str(), &Vars::Visuals::WORLD::Hemp);
									ImGui::SameLine(); ImGui::Text(_xor_("Hemp").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("HempColor##3").c_str(), (float*)&Vars::Visuals::WORLD::HempColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##MushroomESP").c_str(), &Vars::Visuals::WORLD::Mushrooms);
									ImGui::SameLine(); ImGui::Text(_xor_("Mushroom").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("MushroomColor##3").c_str(), (float*)&Vars::Visuals::WORLD::MushroomColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();

								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Loot:").c_str());

									ImGui::Checkbox(_xor_("##CrateESP").c_str(), &Vars::Visuals::WORLD::Crates);
									ImGui::SameLine(); ImGui::Text(_xor_("Crate").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("CrateColor##3").c_str(), (float*)&Vars::Visuals::WORLD::CrateColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##StashESP").c_str(), &Vars::Visuals::WORLD::Stash);
									ImGui::SameLine(); ImGui::Text(_xor_("Stash").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("StashColor##3").c_str(), (float*)&Vars::Visuals::WORLD::StashColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##SupplyESP").c_str(), &Vars::Visuals::WORLD::Supply);
									ImGui::SameLine(); ImGui::Text(_xor_("Supply").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("SupplyColor##3").c_str(), (float*)&Vars::Visuals::WORLD::SupplyColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##BackpackESP").c_str(), &Vars::Visuals::WORLD::Backpack);
									ImGui::SameLine(); ImGui::Text(_xor_("Backpack").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("BackpackColor##3").c_str(), (float*)&Vars::Visuals::WORLD::BackpackColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();

								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);

								ImGui::BeginGroup();
								{
									ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
									ImGui::Text(_xor_("Other:").c_str());

									ImGui::Checkbox(_xor_("##MinicopterESP").c_str(), &Vars::Visuals::WORLD::Minicopter);
									ImGui::SameLine(); ImGui::Text(_xor_("Mini").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("MinicopterColor##3").c_str(), (float*)&Vars::Visuals::WORLD::MinicopterColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

									ImGui::Checkbox(_xor_("##PumpkinESP").c_str(), &Vars::Visuals::WORLD::Pumpkin);
									ImGui::SameLine(); ImGui::Text(_xor_("Pumpkin").c_str()); ImGui::SameLine(85);
									ImGui::ColorEdit4(_xor_("PumpkinColor##3").c_str(), (float*)&Vars::Visuals::WORLD::PumpkinColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
								}
								ImGui::EndGroup();
							}
						}
						ImGui::EndGroup();
					}
					ImGui::Columns();
				}
				ImGui::EndGroup();
			}
			if (menus == 2) {
				ImGui::SetWindowSize(ImVec2{ 238, 262 });
				ImGui::BeginGroup();
				{
					ImGui::Text(_xor_("Aim Features:").c_str());
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);

					ImGui::Checkbox(_xor_("##FovChangerCb").c_str(), &Vars::Misc::FovChanger);
					ImGui::SameLine(); ImGui::Text(_xor_("Fov Changer").c_str());

					if (Vars::Misc::FovChanger) {
						ImGui::SliderFloat(_xor_("##FovChangerSl").c_str(), &Vars::Misc::FovValue, 0, 180);
					}

					ImGui::Checkbox(_xor_("##NoRecoil").c_str(), &Vars::Misc::NoRecoil);
					ImGui::SameLine(); ImGui::Text(_xor_("No Recoil").c_str());

					ImGui::Checkbox(_xor_("##NoSpread").c_str(), &Vars::Misc::NoSpread);
					ImGui::SameLine(); ImGui::Text(_xor_("No Spread").c_str());

					ImGui::Checkbox(_xor_("##SetAutomatic").c_str(), &Vars::Misc::SetAutomatic);
					ImGui::SameLine(); ImGui::Text(_xor_("Set Automatic").c_str());

					ImGui::Checkbox(_xor_("##SuperEoka").c_str(), &Vars::Misc::SuperEoka);
					ImGui::SameLine(); ImGui::Text(_xor_("Super Eoka").c_str());

					ImGui::Checkbox(_xor_("##ChangeSky").c_str(), &Vars::Misc::ChangeSky);
					ImGui::SameLine(); ImGui::Text(_xor_("Change Sky").c_str());

					ImGui::Checkbox(_xor_("##FakeAdmin").c_str(), &Vars::Misc::FakeAdmin);
					ImGui::SameLine(); ImGui::Text(_xor_("Fake Admin").c_str());

					ImGui::Checkbox(_xor_("##SetGravity").c_str(), &Vars::Misc::SetGravity);
					ImGui::SameLine(); ImGui::Text(_xor_("Set Gravity").c_str());

					if (Vars::Misc::SetGravity) {
						ImGui::SliderFloat(_xor_("##SetGravitySl").c_str(), &Vars::Misc::GravityValue, 1.4f, 3.f);
					}
				}
				ImGui::EndGroup();
			}
			
		}
		ImGui::End();
	}
	//DrawPlayersLol();
	ImGui::Render();
}