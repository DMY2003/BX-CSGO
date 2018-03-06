#pragma once

#pragma once

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle() = 0;
	virtual void GetRefEHandle() const = 0;
};

class VectorAligned : public Vector
{
public:
	VectorAligned() {}

	VectorAligned(const Vector &vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

struct Ray_t
{
	Ray_t() { }

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const matrix3x4_t	*m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(Vector vecStart, Vector vecEnd)
	{
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

struct cplane_t
{
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask) = 0;
	virtual TraceType_t	GetTraceType() = 0;
};

class CTraceFilter : public ITraceFilter
{
public:

	bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
	{
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	bool ShouldHitEntity(CBaseEntity *pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2);
	}

	TraceType_t GetTraceType()
	{
		return TRACE_EVERYTHING;
	}

	void *pPassEntity1;
	void *pPassEntity2;
};

class CTraceFilterSkipTeam : public ITraceFilter
{
public:
	bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask)
	{		
		if (pEntityHandle == pLocal || pEntityHandle->GetTeam() == pLocal->GetTeam())
			return false;

		return true;
	}

	TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}

	CBaseEntity* pLocal;
};


typedef bool(*ShouldHitFunc_t)(IHandleEntity *pHandleEntity, int contentsMask);

class CTraceFilterSimple : public CTraceFilter
{
public:
	// It does have a base, but we'll never network anything below here..
	CTraceFilterSimple(const IHandleEntity *passentity, int collisionGroup, ShouldHitFunc_t pExtraShouldHitCheckFn = NULL);
	virtual bool ShouldHitEntity(IHandleEntity *pHandleEntity, int contentsMask);
	virtual void SetPassEntity(const IHandleEntity *pPassEntity) { m_pPassEnt = pPassEntity; }
	virtual void SetCollisionGroup(int iCollisionGroup) { m_collisionGroup = iCollisionGroup; }

	const IHandleEntity *GetPassEntity(void) { return m_pPassEnt; }

private:
	const IHandleEntity *m_pPassEnt;
	int m_collisionGroup;
	ShouldHitFunc_t m_pExtraShouldHitCheckFunction;

};

class CTraceFilterIgnoreWorld : public ITraceFilter
{
public:

	bool ShouldHitEntity(CBaseEntity *pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	TraceType_t GetTraceType()
	{
		return TRACE_ENTITIES_ONLY;
	}

	CBaseEntity *pSkip;
};

class CTraceFilterWorldOnly : public ITraceFilter
{
public:

	bool ShouldHitEntity(CBaseEntity *pEntityHandle, int contentsMask)
	{
		return (pEntityHandle && pEntityHandle->GetIndex() == 0);
	}

	TraceType_t GetTraceType() 
	{
		return TRACE_WORLD_ONLY;
	}
};

class CBaseTrace
{
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t
{
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const { return m_pEnt != NULL && !DidHitWorld(); };
	int                     GetEntityIndex() const {
		if (m_pEnt)
			return m_pEnt->GetIndex();
		else
			return -1;
	};
	bool                    DidHit() const { return fraction < 1 || allsolid || startsolid; };
	bool					IsVisible() const;

public:

	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	CBaseEntity*            m_pEnt;
	int                     hitbox;

	CGameTrace() { }

private:
	CGameTrace(const CGameTrace& vOther);
};

typedef CGameTrace trace_t;

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int	GetPointContents(const Vector &vecAbsPosition, int contentsMask = 0xFFFFFFFF, /*IHandleEntity** */uintptr_t** ppEntity = NULL) = 0;
	virtual int GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = 0) = 0;
	virtual int GetPointContents_Collideable(int *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, int *pEnt, trace_t *pTrace) = 0;
	virtual void ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, int *pCollide, trace_t *pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void        TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace) = 0;
};

/*
class CBaseEntity;

class VectorAligned : public Vector {
public:
	VectorAligned() {}

	VectorAligned(const Vector &vec) {
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}
	float w;
};

struct Ray_t {
	Ray_t() {}

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const void*			m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(Vector vecStart, Vector vecEnd) {
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Zero();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Zero();
		m_Start = vecStart;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
};

enum TraceType_t {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(CBaseEntity* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t	GetTraceType() {
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

struct cplane_t {
	Vector	normal;
	float	dist;
	byte	type;
	byte	signbits;
	byte	pad[2];
};

class CBaseTrace {
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

struct csurface_t {
	const char*		name;
	short			surfaceProps;
	unsigned short	flags;
};

class trace_t : public CBaseTrace {
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	CBaseEntity*            m_pEnt;
	int                     hitbox;

	trace_t() {}

private:
	trace_t(const trace_t& vOther);
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int	GetPointContents(const Vector &vecAbsPosition, int contentsMask = 0xFFFFFFFF, /*IHandleEntity** * /uintptr_t** ppEntity = NULL) = 0;
	virtual int GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = 0) = 0;
	virtual int GetPointContents_Collideable(int *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void ClipRayToEntity(const Ray_t &ray, unsigned int fMask, int *pEnt, trace_t *pTrace) = 0;
	virtual void ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, int *pCollide, trace_t *pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void        TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace) = 0;
};

*/