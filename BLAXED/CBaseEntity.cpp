#include "SDK.h"
#include "Interfaces.h"
#include "Global.h"
#include "Prediction.h"

bool CBaseEntity::SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	PVOID pRenderable = (PVOID)(this + 0x4); //this + 0x4, put this in CBaseEntity, or whatever yours is called.. //IClientRenderable

	typedef bool(__thiscall* SetupBones_t)(PVOID, matrix3x4_t*, int, int, float);
	return Mem::CallVirtualFunction<SetupBones_t>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}


CStudioHdr *CBaseEntity::GetModelPtr()
{	
	return *(CStudioHdr**)((DWORD)this + 0x293C);
}

Vector CBaseEntity::GetBonePos(int bone)
{
	matrix3x4_t boneMatrix[128];

	if (SetupBones(boneMatrix, 128, 0x00000100, 0.0f)) {
		return boneMatrix[bone].at(3);
	}
	return Vector(0, 0, 0);
}

bool CBaseEntity::GetBonePosition(int boneId, Vector &pos)
{
	int id = this->GetIndex();	
	
	if (G::boneCache[id].valid)
	{
		pos = Vector(
			G::boneCache[id].pBoneToWorldOut[boneId][0][3], 
			G::boneCache[id].pBoneToWorldOut[boneId][1][3],
			G::boneCache[id].pBoneToWorldOut[boneId][2][3]
		);

		return true;
	}

	pos = Vector(0, 0, 0);

	return false;
}

Vector CBaseEntity::GetOrigin()
{
	Vector origin = *(Vector*)((uintptr_t)this + m_vecOrigin_);

	if (cfg.Rage.Resolver.prediction)
	{
		// basic prediction
		// using now engine prediction :D
		/*if (this == G::pLocal)
		{
			Vector velocity = this->GetVelocity();
			origin = origin + (velocity * I::pGlobals->interval_per_tick);
		}*/
	}

	return origin;
}

Vector CBaseEntity::GetEyePosition() {
	Vector origin = this->GetOrigin();
	//Vector origin = this->GetAbsOrigin();	
	Vector offset = *(Vector*)((uintptr_t)this + m_vecViewOffset);// - Vector(0, 0, 0.35f);

	//printf("%f: %f %f\n", GetViewOffset().z, (GetAbsOrigin() + GetViewOffset()).z, (GetOrigin() + GetViewOffset()).z);

	return(origin + offset);
}

CBaseCombatWeapon *CBaseEntity::GetWeapon() {
	if (GetActiveWeapon() == -1 || (GetActiveWeapon() & 0xFFF) == -1)
		return NULL;

	CBaseCombatWeapon *weap = (CBaseCombatWeapon*)Interfaces::pClientEntityList->GetClientEntity(this->GetActiveWeapon() & 0xFFF);

	if (!weap)
		return NULL;

	if (!weap->GetOwnerEntity())
		return NULL;

	return weap;
}

UINT* CBaseEntity::GetWeapons() {
	return (UINT*)((DWORD)this + m_hMyWeapons);
}

DWORD CBaseEntity::GetViewModel() {
	return *(DWORD*)((DWORD)this + m_hViewModel);
}

ClientClass* CBaseEntity::GetClientClass()
{
	PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
	typedef ClientClass*(__thiscall* OriginalFn)(PVOID);
	return Mem::CallVirtualFunction<OriginalFn>(pNetworkable, 2)(pNetworkable);
}

int CBaseEntity::GetModelIndex() {
	// DT_BaseEntity -> m_nModelIndex_
	return *(int*)((DWORD)this + m_nModelIndex_);
}

void CBaseEntity::SetModelIndex(int nModelIndex) {
	// DT_BaseEntity -> m_nModelIndex_
	*(int*)((DWORD)this + m_nModelIndex_) = nModelIndex;
}

void CBaseEntity::InvalidateBoneCache()
{
	static DWORD invalidateBoneCache = Mem::FindPatternAimbot("\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx", 0, "client.dll");
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalidateBoneCache + 10);
	*(unsigned int*)((DWORD)this + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)this + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
}
std::array<float, 24> &CBaseEntity::m_flPoseParameter()
{
	return *(std::array<float, 24>*)((uintptr_t)this + m_flPoseParameter_);
}
float* CBaseEntity::GetPoseParameter()
{
	return (float*)((DWORD)this + m_flPoseParameter_); // float[24]?
}

int CBaseEntity::AddToInterpolationList(int a2)
{
	typedef int(__thiscall* AddToInterpolationListFn)(void *ths, int a2);
	static AddToInterpolationListFn AddToInterpolationListReal = NULL;

	if (!AddToInterpolationListReal)
		AddToInterpolationListReal = (AddToInterpolationListFn)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 53 8B 5D 08 B8 ? ? ? ? 57 8B F9 66 39 84 5F"));

	return AddToInterpolationListReal(this, a2);
}

int CBaseEntity::RemoveFromInterpolationList(int a2)
{
	typedef int(__thiscall* RemoveFromInterpolationListFn)(void *ths, int a2);
	static RemoveFromInterpolationListFn RemoveFromInterpolationListReal = NULL;

	if (!RemoveFromInterpolationListReal)
		RemoveFromInterpolationListReal = (RemoveFromInterpolationListFn)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 8B 55 08 0F B7 84 51"));

	return RemoveFromInterpolationListReal(this, a2);
}

bool CBaseEntity::Interpolate(float curtime)
{
	typedef bool(__thiscall* InterpolateFn)(void*,float);
	return Mem::CallVirtualFunction<InterpolateFn>(this, 112)(this, curtime);
}
CCSPlayerAnimState *CBaseEntity::GetPlayerAnimState()
{
	static DWORD addr = NULL;

	if (!addr)
		addr = (DWORD)Mem::FindPattern(xorstr("client.dll"), xorstr("89 87 ? ? ? ? 8B CF 8B 07 8B 80 ? ? ? ? FF D0 33 F6")) + 0x2;
	
	return *(CCSPlayerAnimState**)((DWORD)this + *(DWORD*)addr);
}

Vector CBaseEntity::GetWorldSpaceCenter()
{	
	Vector max = this->GetMaxs() + this->GetAbsOrigin();
	Vector min = this->GetMins() + this->GetAbsOrigin();

	Vector size = max - min;

	size /= 2;
	size += min;

	return size;
}

void CBaseEntity::PVSFix()
{
	//https://www.unknowncheats.me/forum/counterstrike-global-offensive/211594-fix-inaccurate-setupbones-target-player-pvs-fix.html
	
	/**(int*)((DWORD)this + 0xA30) = I::pGlobals->framecount; //we'll skip occlusion checks now
	*(int*)((DWORD)this + 0xA28) = 0;//clear occlusion flags-*/

	*(int*)((DWORD)this + 0xA30) = I::pGlobals->framecount; //we'll skip occlusion checks now
	*(int*)((DWORD)this + 0xA28) = 0;//clear occlusion flags-
}

bool CBaseEntity::CheckOnGround()
{
	bool onGround1 = false;//(fabsf(GetVelocity().z) < 0.1f);
	bool onGround2 = (GetFlags() & FL_ONGROUND);
	bool onGround4 = GetGroundEntityHandle() != -1;
	bool onGround3 = false;

	/*trace_t trace;
	Ray_t ray;
	CTraceFilter filter;

	Vector origin = GetAbsOrigin();

	ray.Init(origin, origin + Vector(0, 0, -1.3f));
	filter.pSkip = this;

	I::pEngineTrace->TraceRay(ray, MASK_SOLID, (ITraceFilter*)&filter, &trace);

	if (trace.fraction < 0.97f)
		onGround3 = true;*/

	//printf("%d %d %.2f %d\n", GetFlags() & FL_ONGROUND, GetGroundEntityHandle(), GetVelocity().z, onGround3);

	return onGround1 || onGround2 || onGround3 || onGround4;
}
VarMapping_t *CBaseEntity::VarMapping()
{
	return reinterpret_cast<VarMapping_t*>((DWORD)this + 0x24);
}
void CBaseEntity::UpdateClientSideAnimation()
{
	typedef void(__thiscall* OriginalFn)(void*);
	return Mem::CallVirtualFunction<OriginalFn>(this, 218)(this);
}
int CBaseEntity::GetSequenceActivity(int sequence)
{
	auto hdr = I::pModelInfo->GetStudioModel((model_t*)this->GetModel());

	if (!hdr)
		return -1;

	// sig for stuidohdr_t version: 53 56 8B F1 8B DA 85 F6 74 55
	// sig for C_BaseAnimating version: 55 8B EC 83 7D 08 FF 56 8B F1 74 3D
	static DWORD getSequenceActivity = (DWORD)Mem::FindPattern("client.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74 3D");
	static auto get_sequence_activity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return get_sequence_activity(this, hdr, sequence);
}
CAnimationLayer *CBaseEntity::GetAnimOverlays()
{
	return *(CAnimationLayer**)((DWORD)this + 10608);
}
int CBaseEntity::GetNumAnimOverlays()
{
	return *(int*)((DWORD)this + 0x297C);
}

CAnimationLayer *CBaseEntity::GetAnimOverlay(int index)
{
	if (index < 15)
		return &GetAnimOverlays()[index];
}

int CBaseEntity::UpdatePlayerAnimState(float eyeYaw, float eyePitch)
{
	typedef void(__thiscall* Fn)(void*, float, float);
	static Fn fn = NULL;

	return 1;
}

bool CBaseEntity::IsBreakableEntity()
{
	using Fn_t = bool(__thiscall*)(CBaseEntity*);

	static Fn_t fn = NULL;

	if (!fn)
		fn = (Fn_t)Mem::FindPattern(xorstr("client.dll"), xorstr("55 8B EC 51 56 8B F1 85 F6 74 68"));

	return fn(this);
}

QAngle& CBaseEntity::GetAbsAngles()
{
	using OriginalFn = QAngle&(__thiscall*)(void*);
	return Mem::CallVirtualFunction<OriginalFn>(this, 11)(this);
}

void CBaseEntity::SetAbsAngles(QAngle angles)
{
	this->GetAbsAngles() = angles;
}

QAngle& CBaseEntity::GetRenderAngles()
{
	// GetRenderAngles
	using GetRenderAnglesFn = QAngle&(__thiscall*)(void*);
	return Mem::CallVirtualFunction<GetRenderAnglesFn>(this + 0x4, 2)(this + 0x4);
}

void CBaseEntity::SetRenderAngles(QAngle angles)
{
	this->GetRenderAngles() = angles;
}

Vector& CBaseEntity::GetRenderOrigin()
{
	// GetRenderAngles
	using GetRenderOriginFn = Vector&(__thiscall*)(void*);
	return Mem::CallVirtualFunction<GetRenderOriginFn>(this + 0x4, 1)(this + 0x4);
}

void CBaseEntity::SetRenderOrigin(Vector origin)
{
	this->GetRenderOrigin() = origin;
}
float CBaseEntity::GetStepSize()
{
	return *(float*)((DWORD)this + 0x300C);
}

void CBaseEntity::SetStepSize(float stepsize)
{
	*(float*)((DWORD)this + 0x300C) = stepsize;
}
float_t CBaseEntity::m_surfaceFriction()
{
	static unsigned int _m_surfaceFriction = Math::FindInDataMap(GetPredDescMap(), "m_surfaceFriction");
	return *(float_t*)((uintptr_t)this + _m_surfaceFriction);
}

void CBaseEntity::GetRenderBounds(Vector& mins, Vector& maxs)
{
	// IClientRenderable::GetRenderBounds
	using GetRenderOriginFn = void(__thiscall*)(void*, Vector& mins, Vector& maxs);
	return Mem::CallVirtualFunction<GetRenderOriginFn>(this + 0x4, 17)(this + 0x4, mins, maxs);
}

const char *CBaseEntity::GetSequenceActivityName(int iSequence)
{
	if (iSequence == -1)
		return "";// xorstr("Not Found!");

	if (!GetModelPtr())
		return "";// xorstr("No model!");

	using OriginalFn = const char*(__fastcall*)(CStudioHdr *pstudiohdr, int iSequence);
	static OriginalFn function = NULL;

	if (!function)
		function = (OriginalFn)Mem::FindPattern(xorstr("client.dll"), (std::string(xorstr("85 C9 74 61 85 D2 78 22 8B 41 04 85 C0 75 0A 8B 01 8B 80 ")) + std::string(xorstr("? ? ? ? EB 03 8B 40 14 3B D0 7D 0A 52 E8 ? ? ? ? 03 40 08"))).c_str());

	CStudioHdr* pStudioHdr = GetModelPtr();

	return function(pStudioHdr, iSequence);
}

void CBaseEntity::DrawModel(int flags, BYTE alpha)
{
	// GetRenderAngles
	using Draw_t = void(__thiscall*)(void*, int, BYTE);
	return Mem::CallVirtualFunction<Draw_t>(this + 0x4, 9)(this + 0x4, flags, alpha);
}

bool CBaseEntity::ShouldDraw()
{
	// GetRenderAngles
	using ShouldDraw_t = bool(__thiscall*)(void*);
	return Mem::CallVirtualFunction<ShouldDraw_t>(this + 0x4, 3)(this + 0x4);
}
