#pragma once
#define COLOUR(x) x/255
#include "xor.h"

namespace Vars
{
	int TestWidth;
	int TestHeight;
	int OtherTestWidth;
	int OtherTestHeight;
	int lineWidth = 10;

	namespace Screen
	{
		static int Width = GetSystemMetrics(SM_CXSCREEN);
		static int Height = GetSystemMetrics(SM_CYSCREEN);
	}

	namespace Aim
	{
		static bool bSmoothing = false;
		static int iSmoothing = 10;
		static bool Prediction;
		static bool VisibleCheck = true;
		static 	bool IgnoreTeam = false;
		static bool AimBot;
		static bool SilentAim;
		static int BoneToAim = 7;
		ImColor DrawFovColor = ImVec4(0.f, 1.f, 1.f, 1.f);
		static int DrawFovValue = 200;
		ImColor DrawCrosshairColor = ImVec4(COLOUR(245.0f), COLOUR(66.0f), COLOUR(66.0f), 1.f);
		static bool Crosshair;
	}

	namespace Recoil {
		std::vector<uintptr_t> Weapons {};
		std::vector<uintptr_t> Automatics {};
		std::vector<uintptr_t> Spreads {};
	}

	namespace Visuals
	{
		static bool DrawFov;
		static bool TeamCheck;
		static bool ShowPredictionCircle;

		static int PrefabEspRange = 100;
		static bool PrefabEsp;

		static bool OnlyShowPlayers = true;

		static bool BeltItemsBool;
		static bool ClothItemsBool;
		static bool ShowClothes;

		static bool VisCheck;
		static bool Line;

		namespace Players {

			static int PlayerESPRange = 100;
			static bool Skeleton;
			static bool PlayerESP;
			static bool Box;
			static bool HP;
			static bool FilledBox;
			static bool IgnoreSleeping;
			static bool DisplayName;
			static bool ActiveESP;
			static bool Distance;

			static ImVec4 NameEspColor = ImVec4(COLOUR(255.0f), COLOUR(255.0f), COLOUR(255.0f), 1.f); // Light Blue
			static ImVec4 BoxEspColor = ImVec4(COLOUR(0.0f), COLOUR(255.0f), COLOUR(163.0f), 1.f); // Light Red
			static ImVec4 FilledEspColor = ImVec4(COLOUR(73.0f), COLOUR(73.0f), COLOUR(73.0f), COLOUR(150.0f)); // Light Grey
			static ImVec4 InHpBarColor = ImVec4(COLOUR(255.0f), COLOUR(66.0f), COLOUR(66.0f), 1.f); // Red
			static ImVec4 OutHpBarColor = ImVec4(COLOUR(74.0f), COLOUR(255.0f), COLOUR(104.0f), 1.f); // Green
			static ImVec4 SleepEspColor = ImVec4(COLOUR(255.f), COLOUR(255.f), COLOUR(255.f), 1.f);
			static ImVec4 ActiveEspColor = ImVec4(COLOUR(255.f), COLOUR(0.f), COLOUR(0.f), 1.f);
			static ImVec4 DistanceEspColor = ImVec4(COLOUR(255.f), COLOUR(255.f), COLOUR(255.f), 1.f);
			static ImVec4 SkeletonEspColor = ImVec4(COLOUR(255.f), COLOUR(255.f), COLOUR(255.f), 1.f);

		}

		static bool Weapon;
		static bool CornerBox;
		static bool HeadCircle;
		static bool Bones;
		static int FOV = 90;

		namespace WORLD {
			
			static bool ShowDistance;
			namespace Ores {

				static bool Sulfur;
				static ImVec4 SulfurColor = ImVec4(COLOUR(255.0f), COLOUR(237.0f), COLOUR(122.0f), 1.f);

				static bool Stone;
				static ImVec4 StoneColor = ImVec4(COLOUR(227.0f), COLOUR(227.0f), COLOUR(227.0f), 1.f);

				static bool Metal;
				static ImVec4 MetalColor = ImVec4(COLOUR(227.0f), COLOUR(227.0f), COLOUR(227.0f), 1.f);

			}

			static bool Pumpkin;
			static ImVec4 PumpkinColor = ImVec4(COLOUR(255.0f), COLOUR(181.0f), COLOUR(112.0f), 1.f);

			static bool Minicopter;
			static ImVec4 MinicopterColor = ImVec4(COLOUR(255.0f), COLOUR(181.0f), COLOUR(112.0f), 1.f);

			static bool Supply;
			static ImVec4 SupplyColor = ImVec4(COLOUR(60.0f), COLOUR(102.0f), COLOUR(64.0f), 1.f);

			static bool Hemp;
			static ImVec4 HempColor = ImVec4(COLOUR(84.0f), COLOUR(209.0f), COLOUR(96.0f), 1.f);

			static bool Stash;
			static ImVec4 StashColor = ImVec4(COLOUR(0.f), COLOUR(255.f), COLOUR(188.f), 1.f);

			static bool Wood;
			static ImVec4 WoodColor = ImVec4(COLOUR(163.f), COLOUR(73.f), COLOUR(73.f), 1.f);

			static bool Crates;
			static ImVec4 CrateColor = ImVec4(COLOUR(255.f), COLOUR(255.f), COLOUR(255.f), 1.f);

			static bool Backpack;
			static ImVec4 BackpackColor = ImVec4(COLOUR(255.f), COLOUR(255.f), COLOUR(255.f), 1.f);

			static bool Mushrooms;
			static ImVec4 MushroomColor = ImVec4(COLOUR(255.f), COLOUR(85.f), COLOUR(85.f), 1.f);

			namespace NPC {
				static bool Bear;
				static bool Wolf;
			}
			static bool AK47 = true;
		}
	}

	namespace Prefabs {
		namespace SulfurOre {
			std::string Name = _xor_("Sulfur Ore");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/resource/ores/sulfur-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_sand/sulfur-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_snow/sulfur-ore.prefab")
			};
		}
		namespace SulfurCol {
			std::string Name = _xor_("Sulfur (collectable)");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/stone/sulfur-collectable.prefab"),
			};
		}
		namespace StoneOre {
			std::string Name = _xor_("Stone Ore");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/resource/ores/stone-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_sand/stone-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_snow/stone-ore.prefab")
			};
		}
		namespace StoneCol {
			std::string Name = _xor_("Stone (collectable)");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/stone/stone-collectable.prefab"),
			};
		}
		namespace MetalOre {
			std::string Name = _xor_("Metal Ore");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/resource/ores/metal-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_sand/metal-ore.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/resource/ores_snow/metal-ore.prefab")
			};
		}
		namespace MetalCol {
			std::string Name = _xor_("Metal (collectable)");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/stone/metal-collectable.prefab"),
			};
		}
		namespace Backpack {
			std::string Name = _xor_("Backpack");
			std::vector<std::string> Prefabs{
				_xor_("assets/prefabs/misc/item drop/item_drop_backpack.prefab"),
			};
		}
		namespace WoodCol {
			std::string Name = _xor_("Wood (collectable)");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/wood/wood-collectable.prefab"),
			};
		}
		namespace Military {
			std::string Name = _xor_("Military");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/radtown/crate_normal.prefab"),
			};
		}
		namespace Crates {
			std::string Name = _xor_("Crate");
			std::vector<std::string> Prefabs {
				_xor_("assets/bundled/prefabs/radtown/crate_normal_2.prefab"),
				_xor_("assets/bundled/prefabs/radtown/crate_normal_2_food.prefab"),
				_xor_("assets/bundled/prefabs/radtown/crate_normal_2_medical.prefab")
			};
		}
		namespace Hemp {
			std::string Name = _xor_("Hemp");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/hemp/hemp-collectable.prefab"),
			};
		}
		namespace Stash {
			std::string Name = _xor_("Stash");
			std::vector<std::string> Prefabs{
				_xor_("assets/prefabs/deployable/small stash/small_stash_deployed.prefab"),
			};
		}
		namespace Mushroom {
			std::string Name = _xor_("Mushroom");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-5.prefab"),
				_xor_("assets/bundled/prefabs/autospawn/collectable/mushrooms/mushroom-cluster-6.prefab")
			};
		}
		namespace Supply {
			std::string Name = _xor_("Supply Drop");
			std::vector<std::string> Prefabs{
				_xor_("assets/prefabs/misc/supply drop/supply_drop.prefab")
			};
		}
		namespace Pumpkin {
			std::string Name = _xor_("Pumpkin");
			std::vector<std::string> Prefabs{
				_xor_("assets/bundled/prefabs/autospawn/collectable/pumpkin/pumpkin-collectable.prefab")
			};
		}
	}

	namespace Misc
	{
		static float OldFovValue;
		static float FovValue;
		static bool FovChanger;

		static bool FakeAdmin;
		static int isFakeAdmin;

		static bool NoSpread;
		static bool SetAutomatic;

		static bool NoRecoil;
		static bool Spyderman;

		static bool ChangeSky;
		static int isChangedSky = 0;

		static bool SetGravity;
		static float GravityValue = 1.4f;
		static float OldGravityValue = 0.f;
		
		static bool FatBullet;
		static bool isFatBullet;

		static bool SuperEoka;
	}
}

