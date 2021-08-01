
#pragma once
#include <stdint.h>
#include <Windows.h>
DWORD64 automatic[]
{
	1545779598, // AK - 47;
	-1758372725, // Òîìñîí
	1796682209, // SMG
	-1812555177,// LR - 300
	1318558775, // MP5
	-2069578888, // M249
};

DWORD64 sniper[]
{
	-778367295, // L96
	1588298435, // Áîëò
};

DWORD64 semiautomatic[]{
	-904863145, // Áåðäàíêà 
	818877484, // Ïýøêà
	-852563019, // áåðåòà
	1373971859, // Ïèòîí
	649912614, // ðåâîëüâåð
	28201841, // M39
	1953903201, // Ãâîçäîì¸ò
	-1123473824, // Ãðàíàòîì¸ò
};

DWORD64 other[]{
	1802482880, // bow

};

DWORD64 bow = 1802482880;

UINT64 GABase;
UINT64 UPBase;

#define oGameObjectManager 0x17C1F18
#define oBaseNetworkable 0x3234020

//#define oBaseNetworkable   0x3115078 // "Name": "Class$BaseNetworkable","Signature" : "BaseNetworkable_c*"


//class BasePlayer
//new player flags 650

#define oPlayerFlags 0x650 // public BasePlayer.PlayerFlags playerFlags;
#define oPlayerName 0x6B0 // protected string _displayName;
#define oPlayerHealth 0x224 // private float _health;
#define olastSentTickTime 0x624 // private float lastSentTickTime;
#define oClientTeam 0x598 // public PlayerTeam clientTeam;
#define oLifestate 0x21C // public BaseCombatEntity.LifeState lifestate;
#define oSteamID 0x698 // public ulong userID;
#define oPlayerInventory 0x660 // public PlayerInventory inventory;
#define oActiveUID 0x5C8 // private uint clActiveItem;
#define oFrozen 0x4C8 // public bool Frozen;
#define oWaterBonus 0x728 // public float clothingWaterSpeedBonus;
#define oNoBlockAiming 0x720 // public bool clothingBlocksAiming;
#define oSpeedReduction 0x724 // public float clothingMoveSpeedReduction;
#define oClothingAccuracyBonus 0x72C //	public float clothingAccuracyBonus;
#define oIsSleeping 0x521 // fiendoMomonte

//DISPLAY NAME
#define oDisplayName 0x6B0
#define oDisplayNameLength 0x10
#define oDisplayNameString 0x14

//1
#define oContainerBelt 0x28 // public ItemContainer containerBelt
#define oBeltContentList 0x38 // public List<Item> contents

//EntityRef
#define oHeld 0x98 // private EntityRef heldEntity
#define oStancePenalty 0x31C // private float stancePenalty;
#define	oAimconePenalty	0x320 // private float aimconePenalty;
#define	oHipAimCone 0x2EC // public float hipAimCone;
#define	oAimCone 0x2E8 // public float aimCone;			(class BaseProjectile)
#define	oAimconePenaltyPerShot 0x2F0 // public float aimconePenaltyPerShot;
#define oSuccessFraction 0x360 // public float successFraction;
#define oAttackRadius 0x294 // public float attackRadius;
#define oEffectiveRange 0x1FC // public float effectiveRange;
#define oIsAutomatic 0x298 // public bool isAutomatic;
#define oMaxDistance 0x290 // public float maxDistance; (class BaseMelee)
#define oRepeatDelay 0x1F4 // public float repeatDelay;

// Bone
#define oBoneTransforms 0x48 //public Transform[] boneTransforms;

//BaseProjectile
#define oAuto 0x288 // public bool automatic;
#define oFastReload 0x2C0 // bool fractionalReload
#define oDistance 0x280 // public float distanceScale
#define oprojectileVelocityScale 0x284 // public float projectileVelocityScale

// FOV
#define m_Graphics 0x3231F60

//BaseMovement
#define oPlayerMovement 0x4E8 // public BaseMovement movement;
//-> Gravity
#define oGravityMultiplier 0x7C // public float gravityMultiplier;
#define oGroundAngleNew 0xC0 // private float groundAngleNew;

//RecoilProperties
#define oRecoilProperties 0x2D8 // public RecoilProperties recoil;
#define oRecoilMinYaw 0x18 // public float recoilYawMin;
#define oRecoilMaxYaw 0x1C // public float recoilYawMax;
#define oRecoilMinPitch 0x20 // public float recoilPitchMin;
#define oRecoilMaxPitch 0x24 // public float recoilPitchMax;

// No Spread
#define oAimSway 0x2D0 // public float aimSway;
#define oAimSwaySpeed 0x2D4 // public float aimSwaySpeed;

//PlayerModel
#define oPlayerModel 0x4C0 // public PlayerModel playerModel;
#define oVisible 0x268 // internal bool visible;

//PlayerInput - AIMBOT
#define oPlayerInput 0x4E0 // public PlayerInput input;
#define oBodyAngles 0x3C // private Vector3 bodyAngles;
#define oRecoilAngles 0x64 // public Vector3 recoilAngles;
#define oHasKeyFocus 0x94 // private bool hasKeyFocus;

//Player Eyes
#define oPlayerEyes 0x658 // public PlayerEyes eyes;
#define oArrowBack 0x364 // private float arrowBack;
#define oTriggerReady 0x360 // protected bool attackReady;
#define oItemDefinition 0x20 // public ItemDefinition info;
#define oItemid 0x18 // public int itemid;

#ifndef _OFFSET_HPP
#define _OFFSET_HPP

/* A class for interacting with the Offsets */
class Offsets
{
public:

	/* Constructor for this class */
	Offsets()
	{

	}

public:

	/*
	 * Game Object Manager ( GOM )
	 * UnityPlayer.dll + Offset
	 */
	uint64_t gameObjectManager = 0x17C1F18;

	/**
	 * protected string _displayName
	 * BasePlayer + Offset
	 */
	uint64_t playerDisplayName = 0x6B0;

	/**
	 * public ulong userID
	 * BasePlayer + Offset
	 */
	uint64_t playerUserID = 0x698;

	/**
	 * public PlayerModel playerModel
	 * BasePlayer + Offset
	 */
	uint64_t playerModel = 0x4C0;

	/**
	 * private ModelState modelState
	 * PlayerModel + Offset
	 */
	uint64_t modelState = 0x1F0;

	/**
	 * public float waterLevel;
	 * ModelState + Offset
	 */
	uint64_t waterLevel = 0x14;

	/**
	 * public int flags
	 * ModelState + Offset
	 */
	uint64_t modelFlags = 0x24;

	/**
	 * public Model model
	 * BaseEntity + Offset
	 */
	uint64_t entityModel = 0x128;

	/**
	 * internal bool isLocalPlayer
	 * PlayerModel + Offset
	 */
	uint64_t isLocalPlayer = 0x279;

	/**
	 * public Transform[] boneTransforms
	 * Model + Offset
	 */
	uint64_t boneTransforms = 0x48;

	/**
	 * private Vector3 newVelocity
	 * PlayerModel + Offset
	 */
	uint64_t playerVelocity = 0x21C;

	/**
	 * public bool adminCheat
	 * BaseMovement + Offset
	 */
	uint64_t adminCheat = 0x18;

	/**
	 * public BaseMovement movement
	 * BasePlayer + Offset
	 */
	uint64_t playerBaseMovement = 0x4E8;

	/**
	 * public float clothingMoveSpeedReduction
	 * BasePlayer + Offset
	 */
	uint64_t clothingMoveSpeedReduction = 0x724;

	/**
	 * public float timeScale;
	 * LTSeq + Offset
	 */
	uint64_t timeScale = 0x2C;

	/**
	 * private float groundAngle
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundAngle = 0xBC;

	/**
	 * private float groundAngleNew
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundAngleNew = 0xC0;

	/**
	 * public float maxAngleClimbing;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t maxAngleClimbing = 0x88;

	/**
	 * public float maxAngleWalking;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t maxAngleWalking = 0x84;

	/**
	 * public float groundTime;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundTime = 0xC4;

	/**
	 * public PlayerEyes eyes;
	 * BasePlayer + offset
	 */
	uint64_t playerEyes = 0x658;

	/**
	 * private Vector3 viewOffset
	 * PlayerEyes + offset
	 */
	uint64_t ViewOffset = 0x38;

	/**
	 * public static readonly Vector3 EyeOffset;
	 * PlayerEyes + offset
	 */
	uint64_t EyeOffset = 0x0;


	uint64_t EyeBodyRot = 0x44; //Quateranion

	/**
	 * public PlayerInput input
	 * BasePlayer + Offset
	 */
	uint64_t playerInput = 0x4E0;

	/**
	 * public float clientTickInterval
	 * BasePlayer + Offset
	 */
	uint64_t clientTickInterval = 0x620;

	/**
	 * public InputState state;
	 * Input + Offset
	 */
	uint64_t state = 0x20;

	/**
	 * public InputMessage current;
	 * state + Offset
	 */
	uint64_t current = 0x10;

	/**
	 * public Vector3 aimAngles;
	 * current + Offset
	 */
	uint64_t aimAngles = 0x18;

	/**
	 * public Vector3 recoilAngles
	 * PlayerInput + Offset
	 */
	uint64_t recoilAngles = 0x64;

	/**
	 * private Vector3 bodyAngles
	 * PlayerInput + Offset
	 */
	uint64_t bodyAngles = 0x3C;

	/**
	 * private float _health
	 * BaseCombatEntity + Offset
	 */
	uint64_t health = 0x224;

	/**
	 * private EntityRef mounted
	 * BasePlayer + Offset
	 */
	uint64_t mountedEntRef = 0x5E8;

	/**
	 * private uint clActiveItem
	 * BasePlayer + Offset
	 */
	uint64_t clActiveItem = 0x5C8;

	/**
	 * public PlayerInventory inventory
	 * BasePlayer + Offset
	 */
	uint64_t playerInventory = 0x660;

	/**
	 * public ItemContainer containerBelt
	 * PlayerInventory + Offset
	 */
	uint64_t containerBelt = 0x28;

	/**
	 * public List<Item> contents
	 * ItemContainer + Offset
	 */
	uint64_t beltContentsList = 0x38;

	/**
	 * public BasePlayer.PlayerFlags playerFlags
	 * BasePlayer + Offset
	 */
	uint64_t playerFlags = 0x650;

	/**
	 * public float recoilYawMin
	 * RecoilProperties + Offset
	 */
	uint64_t recoilYawMin = 0x18;

	/**
	 * public float recoilYawMax
	 * RecoilProperties + Offset
	 */
	uint64_t recoilYawMax = 0x1C;

	/**
	 * public float recoilPitchMin
	 * RecoilProperties + Offset
	 */
	uint64_t recoilPitchMin = 0x20;

	/**
	 * public float recoilPitchMax
	 * RecoilProperties + Offset
	 */
	uint64_t recoilPitchMax = 0x24;

	/**
	 * public float timeToTakeMin
	 * RecoilProperties + Offset
	 */
	uint64_t timeToTakeMin = 0x28;

	/**
	 * public float timeToTakeMax
	 * RecoilProperties + Offset
	 */
	uint64_t timeToTakeMax = 0x2C;

	/**
	 * public float movementPenalty
	 * BasePlayer + Offset
	 */
	uint64_t movementPenalty = 0x34;

	/**
	 * public float aimCone
	 * BaseProjectile + Offset
	 */
	uint64_t aimCone = 0x2E8;

	/**
	 * public float hipAimCone
	 * BaseProjectile + Offset
	 */
	uint64_t hipAimCone = 0x2EC;

	/**
	 * public float aimConePenaltyMax
	 * BaseProjectile + Offset
	 */
	uint64_t aimConePenaltyMax = 0x2F4;

	/**
	 * public float aimconePenaltyPerShot
	 * BaseProjectile + Offset
	 */
	uint64_t aimconePenaltyPerShot = 0x2F0;

	/**
	 * public float aimconePenaltyRecoverTime
	 * BaseProjectile + Offset
	 */
	uint64_t aimconePenaltyRecoverTime = 0x2F8;

	/**
	 * public float stancePenaltyScale
	 * BaseProjectile + Offset
	 */
	uint64_t stancePenaltyScale = 0x300;
};

extern Offsets* offsets;

#endif // _OFFSET_HPP