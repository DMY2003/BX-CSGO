#pragma once


#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	


// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

#define	MASK_SHOT	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)


#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	SURF_NODECALS	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

typedef struct player_info_s {
private:
	DWORD __pad0[2];
public:
	int m_nXuidLow;
	int m_nXuidHigh;
	char m_szPlayerName[128];
	int m_nUserID;
	char m_szSteamID[33];
	UINT m_nSteam3ID;
	char m_szFriendsName[128];
	bool m_bIsFakePlayer;
	bool m_bIsHLTV;
	DWORD m_dwCustomFiles[4];
	BYTE m_FilesDownloaded;
private:
	int __pad1;
} player_info_t;

enum MoveType
{
	MOVETYPE_NONE = 0,            /**< never moves */
	MOVETYPE_ISOMETRIC,            /**< For players */
	MOVETYPE_WALK,                /**< Player only - moving on the ground */
	MOVETYPE_STEP,                /**< gravity, special edge handling -- monsters use this */
	MOVETYPE_FLY,                /**< No gravity, but still collides with stuff */
	MOVETYPE_FLYGRAVITY,        /**< flies through the air + is affected by gravity */
	MOVETYPE_VPHYSICS,            /**< uses VPHYSICS for simulation */
	MOVETYPE_PUSH,                /**< no clip to world, push and crush */
	MOVETYPE_NOCLIP,            /**< No gravity, no collisions, still do velocity/avelocity */
	MOVETYPE_LADDER,            /**< Used by players only when going onto a ladder */
	MOVETYPE_OBSERVER,            /**< Observer movement, depends on player's observer mode */
	MOVETYPE_CUSTOM,            /**< Allows the entity to describe its own physics */
};

/*struct player_info_t
{
	char __pad0[0x8];

	int xuidlow;
	int xuidhigh;

	char name[128];
	int userid;
	char guid[33];

	char __pad1[0x17B];
};*/


#define METERS_PER_INCH (0.0254f)

enum CSGOClassId : int
{
	_CAI_BaseNPC,
	_CAK47,
	_CBaseAnimating,
	_CBaseAnimatingOverlay,
	_CBaseAttributableItem,
	_CBaseButton,
	_CBaseCombatCharacter,
	_CBaseCombatWeapon,
	_CBaseCSGrenade,
	_CBaseCSGrenadeProjectile,
	_CBaseDoor,
	_CBaseEntity,
	_CBaseFlex,
	_CBaseGrenade,
	_CBaseParticleEntity,
	_CBasePlayer,
	_CBasePropDoor,
	_CBaseTeamObjectiveResource,
	_CBaseTempEntity,
	_CBaseToggle,
	_CBaseTrigger,
	_CBaseViewModel,
	_CBaseVPhysicsTrigger,
	_CBaseWeaponWorldModel,
	_CBeam,
	_CBeamSpotlight,
	_CBoneFollower,
	_CBreakableProp,
	_CBreakableSurface,
	_CC4,
	_CCascadeLight,
	_CChicken,
	_CColorCorrection,
	_CColorCorrectionVolume,
	_CCSGameRulesProxy,
	_CCSPlayer,
	_CCSPlayerResource,
	_CCSRagdoll,
	_CCSTeam,
	_CDEagle,
	_CDecoyGrenade,
	_CDecoyProjectile,
	_CDynamicLight,
	_CDynamicProp,
	_CEconEntity,
	_CEconWearable,
	_CEmbers,
	_CEntityDissolve,
	_CEntityFlame,
	_CEntityFreezing,
	_CEntityParticleTrail,
	_CEnvAmbientLight,
	_CEnvDetailController,
	_CEnvDOFController,
	_CEnvParticleScript,
	_CEnvProjectedTexture,
	_CEnvQuadraticBeam,
	_CEnvScreenEffect,
	_CEnvScreenOverlay,
	_CEnvTonemapController,
	_CEnvWind,
	_CFEPlayerDecal,
	_CFireCrackerBlast,
	_CFireSmoke,
	_CFireTrail,
	_CFish,
	_CFlashbang,
	_CFogController,
	_CFootstepControl,
	_CFunc_Dust,
	_CFunc_LOD,
	_CFuncAreaPortalWindow,
	_CFuncBrush,
	_CFuncConveyor,
	_CFuncLadder,
	_CFuncMonitor,
	_CFuncMoveLinear,
	_CFuncOccluder,
	_CFuncReflectiveGlass,
	_CFuncRotating,
	_CFuncSmokeVolume,
	_CFuncTrackTrain,
	_CGameRulesProxy,
	_CHandleTest,
	_CHEGrenade,
	_CHostage,
	_CHostageCarriableProp,
	_CIncendiaryGrenade,
	_CInferno,
	_CInfoLadderDismount,
	_CInfoOverlayAccessor,
	_CItem_Healthshot,
	_CItemDogtags,
	_CKnife,
	_CKnifeGG,
	_CLightGlow,
	_CMaterialModifyControl,
	_CMolotovGrenade,
	_CMolotovProjectile,
	_CMovieDisplay,
	_CParticleFire,
	_CParticlePerformanceMonitor,
	_CParticleSystem,
	_CPhysBox,
	_CPhysBoxMultiplayer,
	_CPhysicsProp,
	_CPhysicsPropMultiplayer,
	_CPhysMagnet,
	_CPlantedC4,
	_CPlasma,
	_CPlayerResource,
	_CPointCamera,
	_CPointCommentaryNode,
	_CPointWorldText,
	_CPoseController,
	_CPostProcessController,
	_CPrecipitation,
	_CPrecipitationBlocker,
	_CPredictedViewModel,
	_CProp_Hallucination,
	_CPropDoorRotating,
	_CPropJeep,
	_CPropVehicleDriveable,
	_CRagdollManager,
	_CRagdollProp,
	_CRagdollPropAttached,
	_CRopeKeyframe,
	_CSCAR17,
	_CSceneEntity,
	_CSensorGrenade,
	_CSensorGrenadeProjectile,
	_CShadowControl,
	_CSlideshowDisplay,
	_CSmokeGrenade,
	_CSmokeGrenadeProjectile,
	_CSmokeStack,
	_CSpatialEntity,
	_CSpotlightEnd,
	_CSprite,
	_CSpriteOriented,
	_CSpriteTrail,
	_CStatueProp,
	_CSteamJet,
	_CSun,
	_CSunlightShadowControl,
	_CTeam,
	_CTeamplayRoundBasedRulesProxy,
	_CTEArmorRicochet,
	_CTEBaseBeam,
	_CTEBeamEntPoint,
	_CTEBeamEnts,
	_CTEBeamFollow,
	_CTEBeamLaser,
	_CTEBeamPoints,
	_CTEBeamRing,
	_CTEBeamRingPoint,
	_CTEBeamSpline,
	_CTEBloodSprite,
	_CTEBloodStream,
	_CTEBreakModel,
	_CTEBSPDecal,
	_CTEBubbles,
	_CTEBubbleTrail,
	_CTEClientProjectile,
	_CTEDecal,
	_CTEDust,
	_CTEDynamicLight,
	_CTEEffectDispatch,
	_CTEEnergySplash,
	_CTEExplosion,
	_CTEFireBullets,
	_CTEFizz,
	_CTEFootprintDecal,
	_CTEFoundryHelpers,
	_CTEGaussExplosion,
	_CTEGlowSprite,
	_CTEImpact,
	_CTEKillPlayerAttachments,
	_CTELargeFunnel,
	_CTEMetalSparks,
	_CTEMuzzleFlash,
	_CTEParticleSystem,
	_CTEPhysicsProp,
	_CTEPlantBomb,
	_CTEPlayerAnimEvent,
	_CTEPlayerDecal,
	_CTEProjectedDecal,
	_CTERadioIcon,
	_CTEShatterSurface,
	_CTEShowLine,
	_CTesla,
	_CTESmoke,
	_CTESparks,
	_CTESprite,
	_CTESpriteSpray,
	_CTest_ProxyToggle_Networkable,
	_CTestTraceline,
	_CTEWorldDecal,
	_CTriggerPlayerMovement,
	_CTriggerSoundOperator,
	_CVGuiScreen,
	_CVoteController,
	_CWaterBullet,
	_CWaterLODControl,
	_CWeaponAug,
	_CWeaponAWP,
	_CWeaponBaseItem,
	_CWeaponBizon,
	_CWeaponCSBase,
	_CWeaponCSBaseGun,
	_CWeaponCycler,
	_CWeaponElite,
	_CWeaponFamas,
	_CWeaponFiveSeven,
	_CWeaponG3SG1,
	_CWeaponGalil,
	_CWeaponGalilAR,
	_CWeaponGlock,
	_CWeaponHKP2000,
	_CWeaponM249,
	_CWeaponM3,
	_CWeaponM4A1,
	_CWeaponMAC10,
	_CWeaponMag7,
	_CWeaponMP5Navy,
	_CWeaponMP7,
	_CWeaponMP9,
	_CWeaponNegev,
	_CWeaponNOVA,
	_CWeaponP228,
	_CWeaponP250,
	_CWeaponP90,
	_CWeaponSawedoff,
	_CWeaponSCAR20,
	_CWeaponScout,
	_CWeaponSG550,
	_CWeaponSG552,
	_CWeaponSG556,
	_CWeaponSSG08,
	_CWeaponTaser,
	_CWeaponTec9,
	_CWeaponTMP,
	_CWeaponUMP45,
	_CWeaponUSP,
	_CWeaponXM1014,
	_CWorld,
	_DustTrail,
	_MovieExplosion,
	_ParticleSmokeGrenade,
	_RocketTrail,
	_SmokeTrail,
	_SporeExplosion,
	_SporeTrail,
};
