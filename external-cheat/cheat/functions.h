//CLASS BASEPLAYER
#include <Windows.h>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <string.h>
#include <sstream>
#include <limits>
#include <future>
#include <conio.h>
#include <cstdlib>
#include <stdlib.h>
#include <ostream>
#include <regex>

#include "Math.h"
#include "Memory.h"
#include "offsets.h"
#include "Vars.h"

#define safe_write(Addr, Data, Type) driver::write<Type>(Addr, Data)

Vector3 GetPrefabPos(UINT64 PrefabBase) {
	UINT64 Object = driver::read<UINT64>(PrefabBase + 0x10);
	UINT64 ObjectClass = driver::read<UINT64>(Object + 0x30);
	UINT64 GameObject = driver::read<UINT64>(ObjectClass + 0x30);
	UINT64 Transform = driver::read<UINT64>(GameObject + 0x8);
	UINT64 Vec = driver::read<UINT64>(Transform + 0x38);

	return driver::read<Vector3>(Vec + 0x90);
}

std::string GetPrefabName(std::string output) {
	if (Vars::Visuals::WORLD::Ores::Sulfur) {
		for (int i = 0; i < Vars::Prefabs::SulfurOre::Prefabs.size(); i++)
			if (output == Vars::Prefabs::SulfurOre::Prefabs[i]) return Vars::Prefabs::SulfurOre::Name;

		for (int i = 0; i < Vars::Prefabs::SulfurCol::Prefabs.size(); i++)
			if (output == Vars::Prefabs::SulfurCol::Prefabs[i]) return Vars::Prefabs::SulfurCol::Name;
	}
	if (Vars::Visuals::WORLD::Ores::Stone) {
		for (int i = 0; i < Vars::Prefabs::StoneOre::Prefabs.size(); i++)
			if (output == Vars::Prefabs::StoneOre::Prefabs[i]) return Vars::Prefabs::StoneOre::Name;

		for (int i = 0; i < Vars::Prefabs::StoneCol::Prefabs.size(); i++)
			if (output == Vars::Prefabs::StoneCol::Prefabs[i]) return Vars::Prefabs::StoneCol::Name;
	}
	if (Vars::Visuals::WORLD::Ores::Metal) {
		for (int i = 0; i < Vars::Prefabs::MetalOre::Prefabs.size(); i++)
			if (output == Vars::Prefabs::MetalOre::Prefabs[i]) return Vars::Prefabs::MetalOre::Name;

		for (int i = 0; i < Vars::Prefabs::MetalCol::Prefabs.size(); i++)
			if (output == Vars::Prefabs::MetalCol::Prefabs[i]) return Vars::Prefabs::MetalCol::Name;
	}
	if (Vars::Visuals::WORLD::Hemp) {
		for (int i = 0; i < Vars::Prefabs::Hemp::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Hemp::Prefabs[i]) return Vars::Prefabs::Hemp::Name;
	}
	if (Vars::Visuals::WORLD::Wood) {
		for (int i = 0; i < Vars::Prefabs::WoodCol::Prefabs.size(); i++)
			if (output == Vars::Prefabs::WoodCol::Prefabs[i]) return Vars::Prefabs::WoodCol::Name;
	}
	if (Vars::Visuals::WORLD::Backpack) {
		for (int i = 0; i < Vars::Prefabs::Backpack::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Backpack::Prefabs[i]) return Vars::Prefabs::Backpack::Name;
	}
	if (Vars::Visuals::WORLD::Crates) {
		for (int i = 0; i < Vars::Prefabs::Crates::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Crates::Prefabs[i]) return Vars::Prefabs::Crates::Name;

		for (int i = 0; i < Vars::Prefabs::Military::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Military::Prefabs[i]) return Vars::Prefabs::Military::Name;
	}
	if (Vars::Visuals::WORLD::Stash) {
		for (int i = 0; i < Vars::Prefabs::Stash::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Stash::Prefabs[i]) return Vars::Prefabs::Stash::Name;
	}
	if (Vars::Visuals::WORLD::Mushrooms) {
		for (int i = 0; i < Vars::Prefabs::Mushroom::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Mushroom::Prefabs[i]) return Vars::Prefabs::Mushroom::Name;
	}
	if (Vars::Visuals::WORLD::Supply) {
		for (int i = 0; i < Vars::Prefabs::Supply::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Supply::Prefabs[i]) return Vars::Prefabs::Supply::Name;
	}
	if (Vars::Visuals::WORLD::Pumpkin) {
		for (int i = 0; i < Vars::Prefabs::Pumpkin::Prefabs.size(); i++)
			if (output == Vars::Prefabs::Pumpkin::Prefabs[i]) return Vars::Prefabs::Pumpkin::Name;
	}
	if (output == "rifle.ak (world)") {
		return "rifle.ak";
	}
	return "Other";
}

bool IsValidPrefab(std::string output) {
	std::string PrefabName = GetPrefabName(output);
	if (PrefabName != "Other") return true;
	return false;
}

namespace GlobalOther {
	bool IsInRange(Vector3 LocalPos, Vector3 EnemyPos) {
		float DistanceX = LocalPos.x - EnemyPos.x;
		float DistanceY = LocalPos.z - EnemyPos.z;
		float Distance = sqrt((DistanceX * DistanceX) + (DistanceY * DistanceY));
		
		if (Vars::Visuals::PrefabEspRange < Distance) return false;
		else return true;
	}
}

class WeaponData
{
public:
	int GetItemID()
	{
		UINT64 Info = driver::read<UINT64>((const uintptr_t)this + oItemDefinition);
		return driver::read<int>((const uintptr_t)Info + oItemid);
	}
	int GetUID()
	{
		return driver::read<int>((const uintptr_t)this + 0x28);
	}
};

enum BoneList : int
{
	l_hip = 1,
	l_knee,
	l_foot,
	l_toe,
	l_ankle_scale,
	pelvis,
	penis,
	GenitalCensor,
	GenitalCensor_LOD0,
	Inner_LOD0,
	GenitalCensor_LOD1,
	GenitalCensor_LOD2,
	r_hip,
	r_knee,
	r_foot,
	r_toe,
	r_ankle_scale,
	spine1,
	spine1_scale,
	spine2,
	spine3,
	spine4,
	l_clavicle,
	l_upperarm,
	l_forearm,
	l_hand,
	l_index1,
	l_index2,
	l_index3,
	l_little1,
	l_little2,
	l_little3,
	l_middle1,
	l_middle2,
	l_middle3,
	l_prop,
	l_ring1,
	l_ring2,
	l_ring3,
	l_thumb1,
	l_thumb2,
	l_thumb3,
	IKtarget_righthand_min,
	IKtarget_righthand_max,
	l_ulna,
	neck,
	head,
	jaw,
	eyeTranform,
	l_eye,
	l_Eyelid,
	r_eye,
	r_Eyelid,
	r_clavicle,
	r_upperarm,
	r_forearm,
	r_hand,
	r_index1,
	r_index2,
	r_index3,
	r_little1,
	r_little2,
	r_little3,
	r_middle1,
	r_middle2,
	r_middle3,
	r_prop,
	r_ring1,
	r_ring2,
	r_ring3,
	r_thumb1,
	r_thumb2,
	r_thumb3,
	IKtarget_lefthand_min,
	IKtarget_lefthand_max,
	r_ulna,
	l_breast,
	r_breast,
	BoobCensor,
	BreastCensor_LOD0,
	BreastCensor_LOD1,
	BreastCensor_LOD2,
	collision,
	displacement
};

struct TransformAccessReadOnly
{
	uint64_t pTransformData;
};

struct TransformData
{
	uint64_t pTransformArray;
	uint64_t pTransformIndices;
};

struct Matrix34
{
	Vector4 vec0;
	Vector4 vec1;
	Vector4 vec2;
};

Vector3 GetBone(ULONG_PTR pTransform)
{
	__m128 result;

	const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
	const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
	const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

	TransformAccessReadOnly pTransformAccessReadOnly = driver::read<TransformAccessReadOnly>(pTransform + 0x38);


	unsigned int index = driver::read<unsigned int>(pTransform + 0x40);
	//std::cout << "INDEX : " << index << std::endl;

	TransformData transformData = driver::read<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);
	//std::cout << "pTransformArray : " << transformData.pTransformArray << std::endl;
	//std::cout << "pTransformIndices : " << transformData.pTransformIndices << std::endl;

	if (transformData.pTransformArray && transformData.pTransformIndices)
	{
		result = driver::read<__m128>(transformData.pTransformArray + 0x30 * index);
		//std::cout << "result : " << resu << std::endl;
		int transformIndex = driver::read<int>(transformData.pTransformIndices + 0x4 * index);
		//std::cout << "transformIndex : " << transformIndex << std::endl;
		int pSafe = 0;
		while (transformIndex >= 0 && pSafe++ < 200)
		{
			//std::cout << "PSAFE ITERATION : " << pSafe << std::endl;
			Matrix34 matrix34 = driver::read<Matrix34>(transformData.pTransformArray + 0x30 * transformIndex);

			__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));	// xxxx
			__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));	// yyyy
			__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));	// zwxy
			__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));	// wzyw
			__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));	// zzzz
			__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));	// yxwy
			__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

			result = _mm_add_ps(
				_mm_add_ps(
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
								_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
					_mm_add_ps(
						_mm_mul_ps(
							_mm_sub_ps(
								_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
								_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
						tmp7)), *(__m128*)(&matrix34.vec0));

			transformIndex = driver::read<int>(transformData.pTransformIndices + 0x4 * transformIndex);
			//std::cout << "transformIndex2 : " << transformIndex << std::endl;
		}
	}

	Vector3 ReturnValue;
	ReturnValue.x = result.m128_f32[0];
	ReturnValue.y = result.m128_f32[1];
	ReturnValue.z = result.m128_f32[2];

	return ReturnValue;
}

DWORD_PTR GetTransform(int bone, DWORD_PTR entity)
{
	DWORD_PTR PlayerModel = driver::read<DWORD_PTR>(entity + 0x128);
	DWORD_PTR boneTransforms = driver::read<DWORD_PTR>(PlayerModel + oBoneTransforms);
	DWORD_PTR BoneValue = driver::read<DWORD_PTR>(boneTransforms + (0x20 + (bone * 0x8)));
	return driver::read<DWORD_PTR>(BoneValue + 0x10);
};

DWORD_PTR GetTransformModel(int bone, DWORD_PTR PlayerModel)
{
	DWORD_PTR boneTransforms = driver::read<DWORD_PTR>(PlayerModel + oBoneTransforms);
	DWORD_PTR BoneValue = driver::read<DWORD_PTR>(boneTransforms + (0x20 + (bone * 0x8)));
	return driver::read<DWORD_PTR>(BoneValue + 0x10);
};


Vector3 GetBoneByID(BoneList BoneID, DWORD_PTR entity)
{
	return GetBone(GetTransform(BoneID, entity));
}

Vector3 GetBoneByIDModel(BoneList BoneID, DWORD_PTR entity)
{
	return GetBone(GetTransformModel(BoneID, entity));
}

Vector3 GetBoneByIDInt(int BoneID, DWORD_PTR entity)
{
	return GetBone(GetTransform(BoneID, entity));
}

#include <map>

std::string current_weapon = "";
std::map < std::string, std::vector<int> > recoil_map;
std::mutex recoil_mutex;
int yaw_min;
int yaw_max;
int pitch_min;
int pitch_max;

int get_item_id(uintptr_t item)
{
	return driver::read<UINT64>(item + 0x18);
}
std::wstring get_item_name(uintptr_t item)
{
	auto unk = driver::read<UINT64>(item + 0x20);
	unk = driver::read<UINT64>(unk + 0x20);
	return driver::read_wstring(unk + 0x14, 14);
}
bool is_weapon(uintptr_t helditem)
{
	auto ItemName = get_item_name(helditem);
	if (ItemName.find(L"rifle") != std::string::npos) return true;
	if (ItemName.find(L"pistol") != std::string::npos) return true;
	if (ItemName.find(L"bow") != std::string::npos) return true;
	if (ItemName.find(L"lmg") != std::string::npos) return true;
	if (ItemName.find(L"shotgun") != std::string::npos) return true;
	if (ItemName.find(L"smg") != std::string::npos) return true;
	else return false;
}
bool set_recoil_props(uintptr_t weapon)
{
	auto base_projectile = driver::read<uintptr_t>(weapon + oHeld);
	if (!base_projectile) return false;

	auto recoil_prop = driver::read<uintptr_t>(base_projectile + oRecoilProperties);
	if (!recoil_prop) return false;

	driver::write<float>(recoil_prop + oRecoilMinYaw, int(yaw_min));
	driver::write<float>(recoil_prop + oRecoilMaxYaw, int(yaw_max));

	driver::write<float>(recoil_prop + oRecoilMinPitch, int(pitch_min));
	driver::write<float>(recoil_prop + oRecoilMaxPitch, int(pitch_max));

	if (Vars::Misc::NoSpread) {
		driver::write<float>(base_projectile + oAimSway, -1.f); // public float aimSway;
		driver::write<float>(base_projectile + oAimSwaySpeed, -1.f); // public float aimSwaySpeed;
		driver::write<float>(base_projectile + oRecoilProperties, -1.f); // public RecoilProperties recoil;
	}
	return true;
}
bool get_recoil_properties(uintptr_t weapon, std::string name)
{
	auto base_projectile = driver::read<uintptr_t>(weapon + 0x98);
	if (!base_projectile) return false;

	auto recoil_prop = driver::read<uintptr_t>(base_projectile + 0x2C0);
	if (!recoil_prop) return false;

	int yaw_min = driver::read<float>(recoil_prop + 0x18);
	int yaw_max = driver::read<float>(recoil_prop + 0x1C);

	int pitch_min = driver::read<float>(recoil_prop + 0x20);
	int pitch_max = driver::read<float>(recoil_prop + 0x24);

	yaw_min = yaw_min; yaw_max = yaw_max; pitch_min = pitch_min; pitch_max = pitch_max;

	std::lock_guard<std::mutex> guard(recoil_mutex);
	recoil_map[name] = { yaw_min, yaw_max, pitch_min, pitch_max };
}



class BasePlayer
{
public:
	//PREFAB
	UINT64 GetHeldItem(UINT64 LocalPlayer)
	{
		const auto base_player = LocalPlayer;
		const auto active_item_id = driver::read<UINT64>(base_player + oActiveUID);
		const auto player_inventory = driver::read<UINT64>(base_player + oPlayerInventory); 
		const auto inventory_belt = driver::read<UINT64>(player_inventory + oContainerBelt); 
		const auto belt_contents_list = driver::read<UINT64>(inventory_belt + oBeltContentList);
		const auto belt_contents_sz = driver::read<uint32_t>(belt_contents_list + 0x14);
		const auto belt_contents = driver::read<UINT64>(belt_contents_list + 0x10);

		for (auto i = 0u; i <= 6; i++)
		{
			if (belt_contents != 0)
			{
				const auto current_item = driver::read<UINT64>(belt_contents + (0x20 + (i * 8)));
				if (!current_item) continue;
				if (active_item_id == driver::read<uint32_t>(current_item + 0x28)) return current_item;
			}
		}
	}
	Vector3 GetPrefabPos() {
		UINT64 Object = driver::read<UINT64>((uintptr_t)this + 0x10);
		UINT64 ObjectClass = driver::read<UINT64>(Object + 0x30);
		UINT64 GameObject = driver::read<UINT64>(ObjectClass + 0x30);
		UINT64 Transform = driver::read<UINT64>(GameObject + 0x8);
		UINT64 Vec = driver::read<UINT64>(Transform + 0x38);

		return driver::read<Vector3>(Vec + 0x90);
	}

	Vector3 GetBoneID(BoneList id) {
		return GetBoneByID(id, (uintptr_t)this);
	}

	UINT64 GetItemsList() {
		UINT64 Inventory = driver::read<UINT64>((const uintptr_t)this + oPlayerInventory);
		UINT64 Belt = driver::read <UINT64>(Inventory + 0x28);
		UINT64 ItemList = driver::read<UINT64>(Belt + 0x38);
		return driver::read<UINT64>(ItemList + 0x10);
	}

	std::string GetBeltItems(UINT64 ItemsList, int Slot) {
		UINT64 SlotItem;
		if (Slot == 1) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x20);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 2) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x28);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 3) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x30);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 4) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x38);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 5) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x40);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 6) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x48);
			if (!SlotItem) {
				return "None";
			}
		}

		UINT64 Item1Info = driver::read<UINT64>(SlotItem + 0x20);
		if (!Item1Info) {
			return "None";
		}

		UINT64 ItemDisplayName = driver::read<UINT64>(Item1Info + 0x28);
		if (!ItemDisplayName) {
			return "None";
		}

		UINT64 ItemDisplayEnglish = driver::read<UINT64>(ItemDisplayName + 0x18);
		if (!ItemDisplayEnglish) {
			return "None";
		}

		UINT64 NameTest = driver::read<UINT64>(ItemDisplayEnglish + 0x14);

		if (!NameTest) {
			return "None";
		}

		std::wstring Name = driver::ReadUnicode(ItemDisplayEnglish + 0x14);

		std::string output(Name.begin(), Name.end());

		if (output == "dxeso") {
			return "None";
		}
		return output;

	}

	UINT64 GetClothingItemsList() {
		UINT64 Inventory = driver::read<UINT64>((const uintptr_t)this + oPlayerInventory);
		UINT64 Belt = driver::read <UINT64>(Inventory + 0x30);
		UINT64 ItemList = driver::read<UINT64>(Belt + 0x38);
		return driver::read<UINT64>(ItemList + 0x10);
	}

	std::string GetClothingItems(UINT64 ItemsList, int Slot) {
		UINT64 SlotItem;
		if (Slot == 1) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x20);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 2) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x28);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 3) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x30);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 4) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x38);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 5) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x40);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 6) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x48);
			if (!SlotItem) {
				return "None";
			}
		}
		else if (Slot == 7) {
			SlotItem = driver::read<UINT64>(ItemsList + 0x50);
			if (!SlotItem) {
				return "None";
			}
		}

		UINT64 Item1Info = driver::read<UINT64>(SlotItem + 0x20);
		if (!Item1Info) {
			return "None";
		}

		UINT64 ItemDisplayName = driver::read<UINT64>(Item1Info + 0x28);
		if (!ItemDisplayName) {
			return "None";
		}

		UINT64 ItemDisplayEnglish = driver::read<UINT64>(ItemDisplayName + 0x18);
		if (!ItemDisplayEnglish) {
			return "None";
		}

		std::wstring Name = driver::ReadUnicode(ItemDisplayEnglish + 0x14);

		std::string output(Name.begin(), Name.end());
		if (output == "dxeso") {
			return "None";
		}

		return output;

	}

	Vector3 GetBoneInt(int id) {
		return GetBoneByIDInt(id, (uintptr_t)this);
	}

	Vector3 HeadBone() {
		return GetBoneByID(BoneList::head, (uintptr_t)this);
	}

	Vector3 BasePos() {
		DWORD_PTR PlayerModel = driver::read<DWORD_PTR>((uintptr_t)this + oPlayerModel);
		return driver::read<Vector3>(PlayerModel + 0x1F8);
	}

	Vector3 HeadBoneModel(UINT64 Model) {

		return GetBoneByIDModel(BoneList::head, Model);
	}

	Vector3 BasePos(UINT64 PlayaModel) {
		return driver::read<Vector3>(PlayaModel + 0x1F8);
	}
	void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		horizontal = desktop.right;
		vertical = desktop.bottom;
	}
	bool IsInFov(Vector3 ScreenPlayerHead) {
		int horizontal, vertical;
		GetDesktopResolution(horizontal, vertical);
		if (horizontal / 2 - ScreenPlayerHead.x < Vars::Aim::DrawFovValue && horizontal / 2 - ScreenPlayerHead.x > -Vars::Aim::DrawFovValue) {
			if (vertical / 2 - ScreenPlayerHead.y < Vars::Aim::DrawFovValue && vertical / 2 - ScreenPlayerHead.y > -Vars::Aim::DrawFovValue) {
				return true;
			}
		}
		else
		{
			return false;
		}
	}

	UINT64 GetSteamID()
	{
		return driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + oSteamID);
	}

	std::string GetDisplayName() {
		UINT64 DisplayNamePtr = driver::read<UINT64>((const uintptr_t)this + oDisplayName);
		std::wstring Name = driver::ReadUnicode(DisplayNamePtr + oDisplayNameString);
		std::string output(Name.begin(), Name.end());

		return output;
	}

	float GetHealth()
	{
		return driver::read<float>((const uintptr_t)(const uintptr_t)this + oPlayerHealth);
	}

	bool HasFlags(int Flg)
	{
		return driver::read<int>((const uintptr_t)(const uintptr_t)this + oPlayerFlags) & Flg;
	}

	UINT64 GetName() {
		return driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + 0x698);
	}

	std::string GetPName()
	{
		std::string pname = driver::read<std::string>((const uintptr_t)(const uintptr_t)this + oPlayerName);
		return pname;
	}

	bool IsDead()
	{
		if (!(const uintptr_t)this) return true;
		return driver::read<bool>((const uintptr_t)(const uintptr_t)this + oLifestate);
	}

	bool IsSleeping() {
		if ((((Vars::Visuals::Players::IgnoreSleeping) ? !HasFlags(16) : true)))
			return false;
		else
			return true;
	}

	bool IsLocalPlayer() {
		UINT64 PlayerModel = driver::read<UINT64>((const uintptr_t)this + 0x4B0);
		int Response = driver::read<UINT64>(PlayerModel + 0x269);
		
		if (Response == 1) return true;
		return false;
	}

	bool IsPlayer() {
		UINT64 steam = GetSteamID();
		int nums = 0;
		do {
			nums++;
			steam /= 10;
		} while (steam > 0);
		if (nums != 17 /*player*/) return false;
		return true;
	}

	bool IsInRange(Vector3 LocalPos, Vector3 EnemyPos, int Range) {
		float DistanceX = LocalPos.x - EnemyPos.x;
		float DistanceY = LocalPos.z - EnemyPos.z;

		float Distance = sqrt((DistanceX * DistanceX) + (DistanceY * DistanceY));

		if (Vars::Visuals::Players::PlayerESPRange < Distance) return false;
		else return true;
	}

	bool IsInRange(Vector3 LocalPos, Vector3 EnemyPos) {
		float DistanceX = LocalPos.x - EnemyPos.x;
		float DistanceY = LocalPos.z - EnemyPos.z;
		float Distance = sqrt((DistanceX * DistanceX) + (DistanceY * DistanceY));
		
		if (Vars::Visuals::Players::PlayerESPRange < Distance) return false;
		else return true;
	}

	bool WeaponFeatures() {
		const auto current_item = GetHeldItem((const uintptr_t)this);
		bool isWeapon = is_weapon(current_item);

		if (isWeapon) {
			auto base_projectile = driver::read<uintptr_t>(current_item + oHeld);
			auto recoil_prop = driver::read<uintptr_t>(base_projectile + oRecoilProperties);

			if (Vars::Misc::NoRecoil) {
				if (!std::count(Vars::Recoil::Automatics.begin(), Vars::Recoil::Automatics.end(), current_item)) {
					Vars::Recoil::Automatics.push_back(current_item);
					driver::write<float>(recoil_prop + oRecoilMinYaw, int(yaw_min));
					driver::write<float>(recoil_prop + oRecoilMaxYaw, int(yaw_max));
					driver::write<float>(recoil_prop + oRecoilMinPitch, int(pitch_min));
					driver::write<float>(recoil_prop + oRecoilMaxPitch, int(pitch_max));
				}
			}
			if (Vars::Misc::SetAutomatic) {
				if (!std::count(Vars::Recoil::Weapons.begin(), Vars::Recoil::Weapons.end(), current_item)) {
					Vars::Recoil::Weapons.push_back(current_item);
					return driver::write<bool>(base_projectile + oAuto, Vars::Misc::SetAutomatic);
				}
			}
			if (Vars::Misc::NoSpread) {
				if (!std::count(Vars::Recoil::Spreads.begin(), Vars::Recoil::Spreads.end(), current_item)) {
					Vars::Recoil::Spreads.push_back(current_item);
					driver::write<float>(base_projectile + 0x31C, 0.f);
					driver::write<float>(base_projectile + 0x320, 0.f);
					driver::write<float>(base_projectile + 0x2EC, 0.f);
					driver::write<float>(base_projectile + 0x2E8, 0.f);
					driver::write<float>(base_projectile + 0x2F0, 0.f);
				}
			}
			if (Vars::Misc::SuperEoka) {
				// DWORD64 Held = ((uintptr_t)this + oHeld);
				driver::write<float>(base_projectile + oSuccessFraction, 1.f);
			}
		}
	}

	void FatBullet() {
		const auto current_item = GetHeldItem((const uintptr_t)this);
		DWORD64 Held = driver::read<DWORD64>(current_item + oHeld);
		DWORD64 List = driver::read<DWORD64>(Held + 0x358);
		int size = driver::read<DWORD64>(List + 0x18);
		List = driver::read<DWORD64>(List + 0x10);
		{
			for (int i = 0; i < size; ++i)
			{
				UINT64 Item = driver::read<UINT64>(List + 0x20 + (i * 0x8));
				driver::write<float>(Item + 0x2C, 1.0f);
			}
		}
	}

	bool IsValid(bool LPlayer = false)
	{
		if (!this) return false;

		if ((((Vars::Visuals::Players::IgnoreSleeping) ? !HasFlags(16) : true) && !IsDead())) {

			return true;
		}
		else
		{
			return false;
		}	
		return true;
	}
	Vector2 GetRecoilAngles() {
		UINT64 Input = driver::read<UINT64>((const uintptr_t)this + oPlayerInput);
		return driver::read<Vector2>(Input + oRecoilAngles);
	}
	Vector3 GetVelocity()
	{
		UINT64 PlayerModel = driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + oPlayerModel);
		return driver::read<Vector3>(PlayerModel + 0x21C); // base player -> playermod -> velo
	}
	bool IsVisible() {
		if (Vars::Aim::VisibleCheck) {
			UINT64 PlayerModel = driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + oPlayerModel);
			return driver::read<bool>(PlayerModel + 0x258); // internal bool visible; WORK
		}
		else return true;
	}

	/*Fucktion*/
	Vector2 GetVA() {
		UINT64 Input = driver::read<UINT64>((const uintptr_t)this + oPlayerInput);
		return driver::read<Vector2>(Input + oBodyAngles);
	}
	void SetVA(const Vector2& VA)
	{
		UINT64 Input = driver::read<UINT64>((const uintptr_t)this + oPlayerInput);
		driver::write<Vector2>(Input + oBodyAngles, VA);
	}

	void FovChanger(float val) {
		if (Vars::Misc::OldFovValue != val) {
			auto fovConvar = driver::read<uintptr_t>(Gbase + m_Graphics);
			auto m_pGraphicsInstance = driver::read<uintptr_t>(fovConvar + 0xB8);
			driver::write(m_pGraphicsInstance + 0x18, val);
			Vars::Misc::OldFovValue = val;
		}
	}

	void SetAdminFlag()
	{
		int flags = driver::read<int>((const uintptr_t)this + 0x650); //0x638

		flags |= 4;

		driver::write<uintptr_t>((const uintptr_t)this + 0x650, flags);
	}

	void SetGravity(float val) {
		if (Vars::Misc::OldGravityValue != val) {
			auto Movement = driver::read<uintptr_t>((const uintptr_t)this + 0x4E8);
			driver::write(Movement + oGravityMultiplier, val);
			Vars::Misc::OldGravityValue = Vars::Misc::GravityValue;
		}
	}

	int GetTeamSize()
	{
		UINT64 ClientTeam = driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + oClientTeam); // valid
		UINT64 members = driver::read<UINT64>(ClientTeam + 0x30);
		return driver::read<UINT64>(members + 0x18);
	}

	UINT64 IsTeamMate(int Id)
	{
		UINT64 ClientTeam = driver::read<UINT64>((const uintptr_t)(const uintptr_t)this + oClientTeam);
		UINT64 members = driver::read<UINT64>(ClientTeam + 0x30);
		UINT64 List = driver::read<UINT64>(members + 0x10);
		UINT64 player = driver::read<UINT64>(List + 0x20 + (Id * 0x8));
		return driver::read<UINT64>(player + 0x20);
	}

	
	WeaponData* GetWeaponInfo(int Id)
	{
		UINT64 Inventory = driver::read<UINT64>((const uintptr_t)this + oPlayerInventory);
		UINT64 Belt = driver::read <UINT64>(Inventory + 0x28);
		UINT64 ItemList = driver::read<UINT64>(Belt + 0x38);
		UINT64 Items = driver::read<UINT64>(ItemList + 0x10);
		return (WeaponData*)driver::read<UINT64>(Items + 0x20 + (Id * 0x8));
	}

	int GetCurrentTeam() {
		return driver::read<int>((uintptr_t)this + 0x578);
	}
	WeaponData* GetActiveWeapon()
	{
		int ActUID = driver::read<int>((const uintptr_t)this + oActiveUID);
		if (!ActUID) return nullptr;
		WeaponData* ActiveWeapon;
		for (int i = 0; i < 6; i++)
			if (ActUID == (ActiveWeapon = GetWeaponInfo(i))->GetUID()) {
				return ActiveWeapon;
			}
		return nullptr;
	}
	std::string GetActiveWeaponText() {
		uint16_t ItemID = this->GetActiveWeapon()->GetItemID();
		if (ItemID == 0xB98E) { //AK
			return "AK-47";
		}
		else if (ItemID == 0x31E1) { //SMG
			return "CUSTOM SMG";
		}
		else if (ItemID == 0x9A57) { //lR
			return "LR-300";
		}
		else if (ItemID == 0x9C37) { //MP5
			return "MP5";
		}
		else if (ItemID == 0xA375) { //Tommy???
			return "TOMMY";
		}
		else if (ItemID == 0xBB78) { //m249
			return "M249";
		}
		else if (ItemID == 0x0EC1) { //l96
			return "L96";
		}
		else if (ItemID == 0x82C3) { //Bolty???
			return "BOLTY";
		}
		else if (ItemID == 0x5371) { //M39
			return "M39";
		}
		else if (ItemID == 0x3261) { //Uknown
			return "UKNOWN 2";
		}
		else if (ItemID == 0x2660) { //Uknown
			return "UKNOWN 1";
		}
		else if (ItemID == 0xE126) { //REVO
			return "REVO";
		}
		else if (ItemID == 0x2593) { //PYTHON
			return "PYTHON";
		}
		else if (ItemID == 0xEBB5) { //m92 pistol
			return "M92";
		}
		else if (ItemID == 0x142C) { //SEMI PISTOL
			return "SEMI PISTOL";
		}
		else if (ItemID == 0xE257) { //SAR
			return "S.A.R.";
		}
		else
		{
			return " ";
		}
	}

private:
};

DWORD64 Vmatrix;
class BaseLocalPlayer {
public:
	BasePlayer* BasePlayer;
	int CurrentTeam;
	static Vector3 WorldToScreen(const Vector3 pos) {
		view_matrix_t matrix = driver::read<view_matrix_t>(Vmatrix + 0x2E4);
		float _x = matrix[0][0] * pos.x + matrix[1][0] * pos.y + matrix[2][0] * pos.z + matrix[3][0];
		float _y = matrix[0][1] * pos.x + matrix[1][1] * pos.y + matrix[2][1] * pos.z + matrix[3][1];

		float w = matrix[0][3] * pos.x + matrix[1][3] * pos.y + matrix[2][3] * pos.z + matrix[3][3];

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = screenX * .5f;
		float y = screenY * .5f;

		x += 0.5f * _x * screenX + 0.5f;
		y -= 0.5f * _y * screenY + 0.5f;

		return { x,y,w };
	}
	static Vector3 FastWorldToScreen(const Vector3 pos, view_matrix_t matrix) {
		//view_matrix_t matrix = driver::read<view_matrix_t>(Vmatrix + 0x2E4);
		float _x = matrix[0][0] * pos.x + matrix[1][0] * pos.y + matrix[2][0] * pos.z + matrix[3][0];
		float _y = matrix[0][1] * pos.x + matrix[1][1] * pos.y + matrix[2][1] * pos.z + matrix[3][1];

		float w = matrix[0][3] * pos.x + matrix[1][3] * pos.y + matrix[2][3] * pos.z + matrix[3][3];

		float inv_w = 1.f / w;
		_x *= inv_w;
		_y *= inv_w;

		float x = screenX * .5f;
		float y = screenY * .5f;

		x += 0.5f * _x * screenX + 0.5f;
		y -= 0.5f * _y * screenY + 0.5f;

		return { x,y,w };
	}

};
BaseLocalPlayer LocalPlayer;
UINT64 MainCamera;

void UpdateLocalPlayer() {
	UINT64 LocalPlayerPtr1 = driver::read<UINT64>(Gbase + oBaseNetworkable);
	UINT64 LocalPlayerPtr2 = driver::read<UINT64>(LocalPlayerPtr1 + 0xB8);
	UINT64 LocalPlayerPtr3 = driver::read<UINT64>(LocalPlayerPtr2 + 0x0);
	UINT64 LocalPlayerPtr4 = driver::read<UINT64>(LocalPlayerPtr3 + 0x10);
	UINT64 LocalPlayerPtr5 = driver::read<UINT64>(LocalPlayerPtr4 + 0x28);
	UINT64 LocalPlayerPtr6 = driver::read<UINT64>(LocalPlayerPtr5 + 0x18);
	UINT64 LocalPlayerPtr7 = driver::read<UINT64>(LocalPlayerPtr6 + 0x20);
	LocalPlayer.BasePlayer = (BasePlayer*)(LocalPlayerPtr7);

	std::cout << LocalPlayer.BasePlayer << std::endl;
	
	UINT64 ViewMatrixPtr1 = driver::read<UINT64>(Ubase + oGameObjectManager);
	UINT64 ViewMatrixPtr2 = driver::read<UINT64>(ViewMatrixPtr1 + 0x8);
	UINT64 ViewMatrixPtr3 = driver::read<UINT64>(ViewMatrixPtr2 + 0x10);
	UINT64 ViewMatrixPtr4 = driver::read<UINT64>(ViewMatrixPtr3 + 0x30);
	Vmatrix = driver::read<UINT64>(ViewMatrixPtr4 + 0x18);

	// 2A18 instead of 2E4
	MainCamera = driver::read<UINT64>(Vmatrix + (0x2E4 + 0x2734));
}

void SetFullBright() {

	driver::write<float>(MainCamera + 0x94, 3.8f);

	UINT64 TodShit = driver::read<UINT64>(MainCamera + 0x88);
	UINT64 Sky = driver::read<UINT64>(TodShit + 0x18);
	UINT64 Night = driver::read<UINT64>(Sky + 0x58);

	//SUN SHIT
	UINT64 Sun = driver::read<UINT64>(Sky + 0x60);
	driver::write<float>(Sun + 0x14, 1.0f); // public float MeshBrightness;
	driver::write<float>(Sun + 0x10, 4.0f); // public float MeshSize;
	driver::write<float>(Sun + 0x18, 4.0f); // public float MeshContrast;

	//Sleep(1);

	//MOON SHIT
	UINT64 Moon = driver::read<UINT64>(Sky + 0x68);
	driver::write<float>(Moon + 0x14, 6.0f); //MESH BRIGHTNESS
	driver::write<float>(Moon + 0x10, 5.0f); //MESH SIZE
	driver::write<float>(Moon + 0x18, 1.0f); //MESH CONTRAST
	driver::write<float>(Moon + 0x24, 5.0f); //POSITION

	//Sleep(1);

	//STARS SHIT
	UINT64 Stars = driver::read<UINT64>(Sky + 0x70);
	driver::write<float>(Stars + 0x10, 1.5f); //MESH SIZE
	driver::write<float>(Stars + 0x14, 20.0f); //MESH BRIGHTNESS

	//Sleep(1);

	//AMBIENT SHIT
	UINT64 Ambient = driver::read<UINT64>(Sky + 0x90);
	driver::write<int>(Ambient + 0x10, 3); //MODE
	driver::write<float>(Ambient + 0x14, 1.2f); //SATURATION

	//Sleep(1);
	//MOON COLOR SHIT
	UINT64 MoonColorPtr = driver::read<UINT64>(Night + 0x10);
	UINT64 MoonColor = driver::read<UINT64>(MoonColorPtr + 0x10);
	driver::write<float>(MoonColor, 1.0f);
	driver::write<float>(MoonColor + 0x4, 1.0f);
	driver::write<float>(MoonColor + 0x8, 0.6f);

	//Sleep(1);
	driver::write<float>(MoonColor + 0xC, 0.6f);
	driver::write<float>(MoonColor + 0x10, 0.6f);

	//Sleep(1);
	driver::write<float>(MoonColor + 0x14, 0.0f);
	driver::write<float>(MoonColor + 0x18, 1.0f);
	driver::write<float>(MoonColor + 0x1C, 1.0f);

	//Sleep(1);
	//SETTING DAY TO NIGHT
	driver::write<UINT64>(Sky + 0x50, Night);

	//NIGHT SHIT
	Vars::Misc::isChangedSky++;
}

float GetBulletSpeed()
{
	uint16_t ItemID = LocalPlayer.BasePlayer->GetActiveWeapon()->GetItemID();
	if (ItemID == 0xB98E) { //AK
		return 344.0f;
	}
	else if (ItemID == 0x31E1) { //SMG
		return 240.0f;
	}
	else if (ItemID == 0x9A57) { //lR
		return 344.0f;
	}
	else if (ItemID == 0x9C37) { //MP5
		return 200.0f;
	}
	else if (ItemID == 0x5C8B) { //Tommy???
		return 277.0f;
	}
	else if (ItemID == 0xBB78) { //m249
		return 488.0f;
	}
	else if (ItemID == 0x0EC1) { //l96
		return 1125.0f;
	}
	else if (ItemID == 0x82C3) { //Bolty???
		return 656.0f;
	}
	else if (ItemID == 0x5371) { //M39
		return 462.0f;
	}
	else if (ItemID == 0x3261) { //Uknown
		return 177.0f;
	}
	else if (ItemID == 0x2660) { //Uknown
		return 166.0f;
	}
	else if (ItemID == 0xE126) { //REVO
		return 270.0f;
	}
	else if (ItemID == 0x2593) { //PYTHON
		return 259.0f;
	}
	else if (ItemID == 0xEBB5) { //m92 pistol
		return 300.0f;
	}
	else if (ItemID == 0x142C) { //SEMI PISTOL
		return 270.0f;
	}
	else if (ItemID == 0xE257) { //SAR
		return 350.0f;
	}
	else if (ItemID == 0x474F) { //bow
		return 56.8f;
	}
	else if (ItemID == 0x3D71) { //COMPOUND
		return 115.6f;
	}
	else if (ItemID == 0x110A) { //crossbow
		return 90.0f;
	}
	else if (ItemID == 0x3261) { //nailgun
		return 50.0f;
	}


	else
	{
		return 300.0f;
	}
	//spas 0xAb32
	//compound = 0x3D71
	//water pipe = 0xEEEB
	//pump shotty = 0x6650
	//eoka = 0x2d2b
	//dub barrel = 0x397F
}

Vector3 Prediction(const Vector3& LP_Pos, BasePlayer* Player, BoneList Bone, Vector3 BonePos)
{
	//Vector3 BonePos = PlayerInfo->HeadBone();
	float Dist = Calc3D_Dist(LP_Pos, BonePos);

	if (Dist > 0.001f) {
		float BulletTime = Dist / GetBulletSpeed();
		//    std::cout << GetBulletSpeed() << endl;
		Vector3 vel = Player->GetVelocity();
		Vector3 PredictVel;
		PredictVel.x = vel.x * BulletTime * 0.75f;
		PredictVel.y = vel.y * BulletTime * 0.75f;
		PredictVel.z = vel.z * BulletTime * 0.75f;

		BonePos.x += PredictVel.x;
		BonePos.y += PredictVel.y;
		BonePos.z += PredictVel.z;
		BonePos.y += (4.905f * BulletTime * BulletTime);
		//BonePos += PredictVel; BonePos.y += (4.905f * BulletTime * BulletTime);
	} return BonePos;
}

void WeaponFix(WeaponData* weapon)
{
	int ItemID = 0;
	if (weapon)
		ItemID = weapon->GetItemID();
	if (!ItemID) return;

}
