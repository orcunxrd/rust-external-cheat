#pragma once
#include "ui.h"
#include "Math.h"
#include "Memory.h"
#include "offsets.h"

#include <vector>
#include "functions.h"

struct InfoPanelPlayer {
    float Health;
    int Distance;
    std::string Name;
    std::string HeldItem;
};

InfoPanelPlayer PanelPlayer;

std::string BeltItems[7];
std::string ClothingItems[8];

struct PlayerBones {

    Vector3 Head;
    Vector3 Neck;

    Vector3 Stomach;

    Vector3 LArm1;
    Vector3 LArm2;
    Vector3 LArm3;

    Vector3 RArm1;
    Vector3 RArm2;
    Vector3 RArm3;

    Vector3 LLeg1;
    Vector3 LLeg2;
    Vector3 LLeg3;

    Vector3 RLeg1;
    Vector3 RLeg2;
    Vector3 RLeg3;

};
struct GoodPrefab {

    std::string PrefabName;
    Vector3 PrefabPos;
    int PrefabDistance;

};
struct GoodPlayers2 {
    UINT64 PlayerBase;

    Vector3 PlayerPos;
    Vector3 PlayerHeadPos;

    float PlayerHealth;
    int PlayerDistance;

    std::string PlayerName;

    bool IsTarget;

    PlayerBones Bones;
};

template <typename Type>
struct ThreadVec
{
    std::vector<Type> vec;
    std::mutex mutex;
};

ThreadVec<GoodPlayers2> GoodPlayers3 = {};
ThreadVec<GoodPrefab> GoodPrefabs = {};

ThreadVec<UINT64> GoodPlayers = {};
ThreadVec<UINT64> discovered = {};

Vector3 LocalPlayerBasePos;

UINT64 client_ents;
uint64_t list_base;

void Update() {
    int EntityListSize = 0;
    while (EntityListSize == 0)
    {
        EntityListSize = driver::read<int32_t>(client_ents + 0x10);
    }
    UpdateLocalPlayer();
}

void EntityDiscovery()
{
    list_base = driver::read<uint64_t>(client_ents + 0x18);
    while (true)
    {
        if (list_base == 0)
            list_base = driver::read<uint64_t>(client_ents + 0x18);

        SleepEx(3000, false);

        std::vector<UINT64> local_players;
        std::vector<GoodPrefab> local_prefabs;
        
        int32_t list_size = driver::read<int32_t>(client_ents + 0x10);
        if (list_size == 0) {
            Update();
        }
        uint64_t* entities = new uint64_t[list_size * sizeof(uint64_t)];

        driver::read(list_base + 0x20, entities, list_size * sizeof(uint64_t));
        std::vector<uint64_t> discovered_entities = std::vector<uint64_t>(entities, entities + list_size);

        for (auto ent : discovered_entities)
        {
            if (Vars::Visuals::PrefabEsp) {

                UINT64 DisplayNamePtr = driver::read <UINT64>(ent + 0x10);
                UINT64 DisplayNamePtr2 = driver::read <UINT64>(DisplayNamePtr + 0x30);
                UINT64 DisplayNamePtr3 = driver::read <UINT64>(DisplayNamePtr2 + 0x60);
                
                if (DisplayNamePtr3) {
                    std::string output = driver::ReadASCII(DisplayNamePtr3);

                    if (IsValidPrefab(output) == true) {
                 
                        GoodPrefab ThisPrefab;
                        Vector3 ThisPrefabPos = GetPrefabPos(ent);
                        int PrefabRange = Calc3D_Dist(LocalPlayerBasePos, ThisPrefabPos);
                        
                        if (PrefabRange < Vars::Visuals::PrefabEspRange) {

                            ThisPrefab.PrefabName = GetPrefabName(output);
                            ThisPrefab.PrefabPos = ThisPrefabPos;
                            ThisPrefab.PrefabDistance = PrefabRange;

                            local_prefabs.push_back(ThisPrefab);
                        }
                    }
                }
            }

            bool NetworkStatus = static_cast<bool>(driver::read<uint64_t>(ent + 0x50));

            if (!NetworkStatus) continue;
            if (ent == (UINT64)LocalPlayer.BasePlayer)
                continue;

            uint64_t GameObject = ReadChain(ent, { 0x10, 0x30 });
            int16_t Tag = driver::read<int16_t>(GameObject + 0x54);

            //here make sure its a player
            if (Tag != 6) continue;

            if (Vars::Visuals::Players::IgnoreSleeping) {
                BasePlayer* DiscoverPlayer = (BasePlayer*)ent;
                if (DiscoverPlayer->IsSleeping()) continue;
            }

            local_players.push_back(ent);
        }

        delete[] entities;

        //save players here daddy
        discovered.mutex.lock();
        discovered.vec = local_players;
        discovered.mutex.unlock();
        
        if (Vars::Visuals::PrefabEsp) {
            GoodPrefabs.mutex.lock();
            GoodPrefabs.vec = local_prefabs;
            GoodPrefabs.mutex.unlock();
        }
    }
}

view_matrix_t FastViewMatrix;

BasePlayer* TempTargetPlayer;
BasePlayer* TargetPlayer;

Vector3 LocalPlayerHead;

Vector3 TargetBonePos;
Vector3 TempTargetBonePos;

std::string PanelName;

#include "silent.h"

void EntityUpdate()
{
    SetPriorityClass(std::this_thread::get_id, REALTIME_PRIORITY_CLASS);
    int horizontal, vertical;
    LocalPlayer.BasePlayer->GetDesktopResolution(horizontal, vertical);

    while (true)
    {
        LocalPlayerHead = LocalPlayer.BasePlayer->HeadBone();
        TempTargetPlayer = 0;
        WeaponData* active = LocalPlayer.BasePlayer->GetActiveWeapon();
        WeaponFix(active);
        SleepEx(10, false);
        float Finish;
        Vector3 Calc = { 0, 0, 0 };
        float Closest = FLT_MAX;
        int ClosestEntity = 1;
        std::vector<GoodPlayers2> updated_players;
        discovered.mutex.lock();
        std::vector<UINT64> players = discovered.vec;
        discovered.mutex.unlock();
        LocalPlayerBasePos = LocalPlayer.BasePlayer->BasePos();

        for (auto entry : players)
        {
            BasePlayer* updated_player = (BasePlayer*)entry;

            if (updated_player == LocalPlayer.BasePlayer)
                continue;
            if (updated_player->IsLocalPlayer())
                continue;

            float updated_player_health = updated_player->GetHealth();
            Vector3 updated_player_pos = updated_player->BasePos();

            int updated_player_distance = Calc3D_Dist(LocalPlayerBasePos, updated_player_pos);

            if (updated_player_distance > Vars::Visuals::Players::PlayerESPRange)
                continue;

            Vector3 updated_player_screen_pos = LocalPlayer.FastWorldToScreen(updated_player_pos, FastViewMatrix);

            if (updated_player_screen_pos.z < 0.01f)
                continue;

            if (updated_player_health < 0.1) continue;
            if (updated_player_pos.x == 0.f || updated_player_pos.z == 0.f) continue;

            GoodPlayers2 ThisPlayer;
            ThisPlayer.PlayerHealth = updated_player_health;
            ThisPlayer.PlayerPos = updated_player_pos;
            ThisPlayer.PlayerDistance = updated_player_distance;
            std::string updated_player_name = updated_player->GetDisplayName();

            if (Vars::Visuals::Players::DisplayName)
                ThisPlayer.PlayerName = updated_player_name;

            Vector3 updated_player_headpos = updated_player->HeadBone();
            ThisPlayer.PlayerHeadPos = updated_player_headpos;

            if (TargetPlayer == updated_player) {
                ThisPlayer.IsTarget = true;

                PanelPlayer.Distance = updated_player_distance;
                PanelPlayer.Name = updated_player_name;
                PanelPlayer.Health = updated_player_health;
                TempTargetBonePos = updated_player_headpos;
            }
            else ThisPlayer.IsTarget = false; 

            Vector3 HeadScreen = LocalPlayer.FastWorldToScreen(updated_player_headpos, FastViewMatrix);
            Finish = pythag(HeadScreen.x, HeadScreen.y, (horizontal / 2), (vertical / 2));
            
            if (Finish < Closest && updated_player->IsInFov(HeadScreen)) {
                Closest = Finish;
                TempTargetPlayer = (BasePlayer*)entry;
            }

            if (Vars::Visuals::Players::Skeleton && updated_player_distance) {

                ThisPlayer.Bones.Head = updated_player_headpos;

                ThisPlayer.Bones.Neck = updated_player->GetBoneInt(46);
                ThisPlayer.Bones.Stomach = updated_player->GetBoneInt(19);

                ThisPlayer.Bones.LArm3 = updated_player->GetBoneInt(26);
                ThisPlayer.Bones.LArm2 = updated_player->GetBoneInt(25);
                ThisPlayer.Bones.LArm1 = updated_player->GetBoneInt(24);


                ThisPlayer.Bones.RArm3 = updated_player->GetBoneInt(57);
                ThisPlayer.Bones.RArm2 = updated_player->GetBoneInt(56);
                ThisPlayer.Bones.RArm1 = updated_player->GetBoneInt(55);

                ThisPlayer.Bones.LLeg3 = updated_player->GetBoneInt(15);
                ThisPlayer.Bones.LLeg2 = updated_player->GetBoneInt(14);
                ThisPlayer.Bones.LLeg1 = updated_player->GetBoneInt(13);

                ThisPlayer.Bones.RLeg3 = updated_player->GetBoneInt(3);
                ThisPlayer.Bones.RLeg2 = updated_player->GetBoneInt(2);
                ThisPlayer.Bones.RLeg1 = updated_player->GetBoneInt(1);
            }

            updated_players.push_back(ThisPlayer);
        }

        TargetBonePos = TempTargetBonePos;
        TargetPlayer = TempTargetPlayer;

        GoodPlayers3.mutex.lock();
        GoodPlayers3.vec = updated_players;
        GoodPlayers3.mutex.unlock();

        if (Vars::Visuals::BeltItemsBool) {
            if (TargetPlayer != 0) {
                UINT64 ItemsList = TargetPlayer->GetItemsList();
                BeltItems[1] = TargetPlayer->GetBeltItems(ItemsList, 1);
                BeltItems[2] = TargetPlayer->GetBeltItems(ItemsList, 2);
                BeltItems[3] = TargetPlayer->GetBeltItems(ItemsList, 3);
                BeltItems[4] = TargetPlayer->GetBeltItems(ItemsList, 4);
                BeltItems[5] = TargetPlayer->GetBeltItems(ItemsList, 5);
                BeltItems[6] = TargetPlayer->GetBeltItems(ItemsList, 6);
            }
        }

        if (Vars::Visuals::ClothItemsBool) {
            if (TargetPlayer != 0) {
                UINT64 ItemsList = TargetPlayer->GetClothingItemsList();
                ClothingItems[1] = TargetPlayer->GetClothingItems(ItemsList, 1);
                ClothingItems[2] = TargetPlayer->GetClothingItems(ItemsList, 2);
                ClothingItems[3] = TargetPlayer->GetClothingItems(ItemsList, 3);
                ClothingItems[4] = TargetPlayer->GetClothingItems(ItemsList, 4);
                ClothingItems[5] = TargetPlayer->GetClothingItems(ItemsList, 5);
                ClothingItems[6] = TargetPlayer->GetClothingItems(ItemsList, 6);
                ClothingItems[7] = TargetPlayer->GetClothingItems(ItemsList, 7);
            }
        }

        // Aimbot
        if (Vars::Aim::AimBot || Vars::Aim::SilentAim && TargetPlayer != 0) {
            if (TargetBonePos.x == 0 || TargetBonePos.y == 0)
                continue;

            if (TargetPlayer->IsDead())
                continue;

            if (Vars::Aim::Prediction && Vars::Aim::AimBot || Vars::Aim::Prediction && Vars::Aim::SilentAim) 
                TargetBonePos = Prediction(LocalPlayerHead, TargetPlayer, penis, TargetBonePos);

            Vector3 TargetAngles = CalcAngle(LocalPlayerHead, TargetBonePos);

            Vector2 Angles; Angles.x = TargetAngles.x, Angles.y = TargetAngles.y;

            if (!Normalize(Angles.y, Angles.x))
                continue;
            if (isnan(Angles.x) || isnan(Angles.y))
                continue;

            // Silent
            if (Vars::Aim::SilentAim) {
                UINT64 PlayerEyes = driver::read<UINT64>((uintptr_t)LocalPlayer.BasePlayer + oPlayerEyes);
                Vector4 QuatAngles = ToQuat({ Angles.x, Angles.y, 0.0f });
                driver::write<Vector4>(PlayerEyes + 0x44, QuatAngles);
            }

            // Aimbot
            if (Vars::Aim::AimBot && GetAsyncKeyState(VK_CAPITAL)) {
                LocalPlayer.BasePlayer->SetVA(Angles);
            }
        } 
    }
}

int Iterations;

void DrawPlayersLol()
{
    int horizontal, vertical;
    LocalPlayer.BasePlayer->GetDesktopResolution(horizontal, vertical);

    timer Time;
    Time.Start();
    FastViewMatrix = driver::read<view_matrix_t>(Vmatrix + 0x2E4);

    if (Vars::Misc::ChangeSky)
        if (Vars::Misc::isChangedSky < 1) SetFullBright();

    if (Vars::Misc::NoRecoil || Vars::Misc::SetAutomatic || Vars::Misc::NoSpread || Vars::Misc::SuperEoka)
        LocalPlayer.BasePlayer->WeaponFeatures();

    if (Vars::Misc::FovChanger)
        LocalPlayer.BasePlayer->FovChanger(Vars::Misc::FovValue);

    if (Vars::Misc::FakeAdmin)
        LocalPlayer.BasePlayer->SetAdminFlag();

    if (Vars::Misc::SetGravity)
        LocalPlayer.BasePlayer->SetGravity(Vars::Misc::GravityValue);

    // Draw Fov
    if (Vars::Visuals::DrawFov) {
        DrawCircleImGui(horizontal / 2, vertical / 2, Vars::Aim::DrawFovValue, Vars::Aim::DrawFovColor, 360);
        DrawCircleImGui(horizontal / 2, vertical / 2, Vars::Aim::DrawFovValue - 1, ImVec4(0.f, 0.f, 0.f, 1.f), 360);
        DrawCircleImGui(horizontal / 2, vertical / 2, Vars::Aim::DrawFovValue + 1, ImVec4(0.f, 0.f, 0.f, 1.f), 360);
    }

    if (Vars::Visuals::PrefabEsp) {
        
        GoodPrefabs.mutex.lock();
        std::vector<GoodPrefab> render_prefabs = GoodPrefabs.vec;
        // std::cout << "render_prefabs Size: " << render_prefabs.size() << std::endl;
        GoodPrefabs.mutex.unlock();

        // Prefab Draw
        for (auto& entry : render_prefabs) {
            
            GoodPrefab DrawPrefab = entry;
            Vector3 PrefabPos = DrawPrefab.PrefabPos;
            
            if (PrefabPos.x == 0.f || PrefabPos.z == 0.f) continue;

            Vector3 PrefabScreenPos = LocalPlayer.FastWorldToScreen(PrefabPos, FastViewMatrix);
            if (PrefabScreenPos.z > 0.1f) {
                
                int PrefabRange = Calc3D_Dist(LocalPlayerBasePos, DrawPrefab.PrefabPos);

                ImVec4 Color;
                std::string PrefabTitle = (Vars::Visuals::WORLD::ShowDistance) ? DrawPrefab.PrefabName + " [" + std::to_string(PrefabRange) + "m]" : DrawPrefab.PrefabName;

                if (DrawPrefab.PrefabName.find(_xor_("Sulfur")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::Ores::SulfurColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Metal")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::Ores::MetalColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Stone")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::Ores::StoneColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Hemp")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::HempColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Mushroom")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::MushroomColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Backpack")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::BackpackColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Supply")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::SupplyColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Wood")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::WoodColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Crate")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::CrateColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Minicopter")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::MinicopterColor;
                else if (DrawPrefab.PrefabName.find(_xor_("Stash")) != std::string::npos)
                    Color = Vars::Visuals::WORLD::StashColor;

                // Old: DrawNewTextImGui
                DrawTextImVec4(PrefabScreenPos.x, PrefabScreenPos.y, Color, PrefabTitle.c_str());
            }

        }
    }

    if (Vars::Visuals::Players::PlayerESP) {
        
        GoodPlayers3.mutex.lock();
        std::vector<GoodPlayers2> render_players = GoodPlayers3.vec;
        GoodPlayers3.mutex.unlock();
        Iterations = 0;

        // std::cout << render_players.size() << std::endl;

        for (auto& entry : render_players)
        {
            ImVec4 TargetCol;
            Iterations++;
            GoodPlayers2 DrawPlayer = entry;

            float DrawPlayerHealth = DrawPlayer.PlayerHealth;
            if (DrawPlayerHealth < 0.1) continue;

            Vector3 PlayerPos = DrawPlayer.PlayerPos;
            Vector3 PlayerHeadPos = DrawPlayer.PlayerHeadPos; 
            PlayerHeadPos.y += .2;
            PlayerPos.y -= .1;
            Vector3 PlayerScreen = LocalPlayer.FastWorldToScreen(PlayerPos, FastViewMatrix);
            Vector3 PlayerHeadScreen = LocalPlayer.FastWorldToScreen(PlayerHeadPos, FastViewMatrix);

            int PlayerDistance = Calc3D_Dist(LocalPlayerBasePos, DrawPlayer.PlayerPos);

            std::string DisplayName;
            int DrawPlayerDistance = DrawPlayer.PlayerDistance;
            (Vars::Visuals::Players::Distance) ? DisplayName = DrawPlayer.PlayerName + " [" + std::to_string(PlayerDistance) + "m]" : DisplayName = DrawPlayer.PlayerName;

            int Height = PlayerScreen.y - PlayerHeadScreen.y;
            int Width = Height / 2;

            Vector3 PrefabScreenPos = LocalPlayer.FastWorldToScreen(DrawPlayer.PlayerPos, FastViewMatrix);

            if (Vars::Visuals::Players::ActiveESP)
                (DrawPlayer.IsTarget) ? TargetCol = Vars::Visuals::Players::ActiveEspColor : TargetCol = Vars::Visuals::Players::BoxEspColor;
            else
                TargetCol = Vars::Visuals::Players::BoxEspColor;

            if (Vars::Visuals::Players::Box) {
                DrawRectImGui(PlayerScreen.x - (Width / 2) - 1, PlayerHeadScreen.y - 1, Width + 2, Height + 2, ImVec4(0.f, 0.f, 0.f, 255.f), 1.3f);
                DrawRectImGui(PlayerScreen.x - (Width / 2), PlayerHeadScreen.y, Width, Height, TargetCol, 1.f);
                DrawRectImGui(PlayerScreen.x - (Width / 2) + 1, PlayerHeadScreen.y + 1, Width - 2, Height - 2, ImVec4(0.f, 0.f, 0.f, 255.f), 1.3f);
            }

            if (Vars::Visuals::Players::HP) {
                if (DrawPlayerHealth > 101) DrawPlayerHealth = 100;
                float HealthHeightCalc = ((float)DrawPlayerHealth / 100) * ((float)Width);

                DrawFilledRectImGui(PlayerScreen.x - (Width / 2), PlayerScreen.y + 1, Width, 3, Vars::Visuals::Players::InHpBarColor);
                DrawFilledRectImGui(PlayerScreen.x - (Width / 2), PlayerScreen.y + 1, HealthHeightCalc, 3, Vars::Visuals::Players::OutHpBarColor);
            }

            if (Vars::Visuals::Players::DisplayName) {
                DrawTextImVec4(PlayerScreen.x - (Width / 2), PlayerHeadScreen.y - 20, Vars::Visuals::Players::NameEspColor, DisplayName.c_str());
            }

            // Line ESP
            // DrawLineImGui(PrefabScreenPos.x, PrefabScreenPos.y, 960, 1080, { 0, 246, 197, 240 }, 1);

            if (Vars::Visuals::Players::Skeleton) {
                ImVec4 ESPCOLOR = ImVec4(255.f, 255.f, 255.f, 255.f);
                Vector3 BoneHead = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.Head, FastViewMatrix);
                Vector3 BoneNeck = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.Neck, FastViewMatrix);
                Vector3 BoneStomach = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.Stomach, FastViewMatrix);
                Vector3 BoneLArm1 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LArm1, FastViewMatrix);
                Vector3 BoneLArm2 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LArm2, FastViewMatrix);
                Vector3 BoneLArm3 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LArm3, FastViewMatrix);
                Vector3 BoneRArm1 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RArm1, FastViewMatrix);
                Vector3 BoneRArm2 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RArm2, FastViewMatrix);
                Vector3 BoneRArm3 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RArm3, FastViewMatrix);
                Vector3 BoneLLeg1 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LLeg1, FastViewMatrix);
                Vector3 BoneLLeg2 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LLeg2, FastViewMatrix);
                Vector3 BoneLLeg3 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.LLeg3, FastViewMatrix);
                Vector3 BoneRLeg1 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RLeg1, FastViewMatrix);
                Vector3 BoneRLeg2 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RLeg2, FastViewMatrix);
                Vector3 BoneRLeg3 = LocalPlayer.FastWorldToScreen(DrawPlayer.Bones.RLeg3, FastViewMatrix);
                if (BoneHead.z > 0.1f && BoneRLeg3.z > 0.1f && BoneRLeg1.z > 0.1f) {
                    DrawLineImGui(BoneHead.x, BoneHead.y, BoneNeck.x, BoneNeck.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneNeck.x, BoneNeck.y, BoneLArm1.x, BoneLArm1.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneLArm1.x, BoneLArm1.y, BoneLArm2.x, BoneLArm2.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneLArm2.x, BoneLArm2.y, BoneLArm3.x, BoneLArm3.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneNeck.x, BoneNeck.y, BoneRArm1.x, BoneRArm1.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneRArm1.x, BoneRArm1.y, BoneRArm2.x, BoneRArm2.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneRArm2.x, BoneRArm2.y, BoneRArm3.x, BoneRArm3.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneNeck.x, BoneNeck.y, BoneStomach.x, BoneStomach.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneStomach.x, BoneStomach.y, BoneLLeg1.x, BoneLLeg1.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneLLeg1.x, BoneLLeg1.y, BoneLLeg2.x, BoneLLeg2.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneLLeg2.x, BoneLLeg2.y, BoneLLeg3.x, BoneLLeg3.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneStomach.x, BoneStomach.y, BoneRLeg1.x, BoneRLeg1.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneRLeg1.x, BoneRLeg1.y, BoneRLeg2.x, BoneRLeg2.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                    DrawLineImGui(BoneRLeg2.x, BoneRLeg2.y, BoneRLeg3.x, BoneRLeg3.y, Vars::Visuals::Players::SkeletonEspColor, 1);
                }
            }

            if (Vars::Visuals::Players::FilledBox) {
                DrawFilledRectImGui(PlayerScreen.x - (Width / 2), PlayerHeadScreen.y, Width, Height, Vars::Visuals::Players::FilledEspColor);
            }
        }
    }

    if (Vars::Visuals::BeltItemsBool) {
        int boxWidth = 85;
        ImVec4 FontColor = ImVec4(COLOUR(255.0f), COLOUR(255.0f), COLOUR(255.f), 255);
        DrawFilledRectImGui(10, (vertical / 2) - (boxWidth / 2), 100, boxWidth, ImVec4(COLOUR(16.0f), COLOUR(12.0f), COLOUR(20.0f), 255.f));
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 17, FontColor, BeltItems[1].c_str());
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 27, FontColor, BeltItems[2].c_str());
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 37, FontColor, BeltItems[3].c_str());
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 47, FontColor, BeltItems[4].c_str());
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 57, FontColor, BeltItems[5].c_str());
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 67, FontColor, BeltItems[6].c_str());
        // DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2) + 75, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), BeltItems[7].c_str());
        DrawFilledRectImGui(10, (vertical / 2) - (boxWidth / 2), 100, 15, ImVec4(COLOUR(24.0f), COLOUR(28.0f), COLOUR(36.0f), 255.f));
        DrawTextImVec4(15, (vertical / 2) - (boxWidth / 2), FontColor, _xor_("Target Belt Items").c_str());
    }

    if (Vars::Visuals::ClothItemsBool) {
        DrawTextImVec4(10, 75, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[0].c_str());
        DrawTextImVec4(10, 85, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[1].c_str());
        DrawTextImVec4(10, 95, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[2].c_str());
        DrawTextImVec4(10, 105, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[3].c_str());
        DrawTextImVec4(10, 115, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[4].c_str());
        DrawTextImVec4(10, 125, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[5].c_str());
        DrawTextImVec4(10, 135, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), ClothingItems[6].c_str());
    }

    if (Vars::Aim::Crosshair) {
        DrawLineImGui((horizontal / 2) - 6, (vertical / 2), (horizontal / 2) - 8, (vertical / 2), Vars::Aim::DrawCrosshairColor, 3);
        DrawLineImGui((horizontal / 2) + 6, (vertical / 2), (horizontal / 2) + 8, (vertical / 2), Vars::Aim::DrawCrosshairColor, 3);

        DrawLineImGui((horizontal / 2), (vertical / 2) + 6, (horizontal / 2), (vertical / 2) + 8, Vars::Aim::DrawCrosshairColor, 3);
        DrawLineImGui((horizontal / 2), (vertical / 2) - 6, (horizontal / 2), (vertical / 2) - 8, Vars::Aim::DrawCrosshairColor, 3);

        DrawCircleImGui((horizontal / 2), (vertical / 2), 1, Vars::Aim::DrawCrosshairColor, 360);
        DrawCircleImGui((horizontal / 2), (vertical / 2), 2, ImVec4(0.f, 0.f, 0.f, 255.f), 360);
    }

    /*int lineWidth = 10;

    DrawLineImGui((horizontal / 2) - lineWidth, (vertical / 2) - 30, (horizontal / 2) + lineWidth, (vertical / 2) - 30, Vars::Visuals::Players::SkeletonEspColor, 1);
    DrawLineImGui((horizontal / 2) - lineWidth, (vertical / 2) + 30, (horizontal / 2) + lineWidth, (vertical / 2) + 30, Vars::Visuals::Players::SkeletonEspColor, 1);
    
    DrawLineImGui((horizontal / 2) - 30, (vertical / 2) - lineWidth, (horizontal / 2) - 30, (vertical / 2) + lineWidth, Vars::Visuals::Players::SkeletonEspColor, 1);
    DrawLineImGui((horizontal / 2) + 30, (vertical / 2) - lineWidth, (horizontal / 2) + 30, (vertical / 2) + lineWidth, Vars::Visuals::Players::SkeletonEspColor, 1);*/

    std::string ContentText = "Paste.PUB BEST FREE CHEATS";

    DrawTextImVec4((horizontal / 2) - 100, 10, ImVec4(COLOUR(255.0f), COLOUR(242.0f), COLOUR(0.f), 255), _xor_("Paste.PUB BEST FREE CHEATS").c_str());
    
    int time = Time.GetDuration();
}