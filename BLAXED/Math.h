#pragma once

#include "Util.h"
#include "datamap.h"

namespace Math
{
	extern inline void SinCos(float radians, float *sine, float *cosine);
	extern void AngleVectors(QAngle q, Vector& vec);
	extern void AngleVectorr(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
	extern void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
	extern inline float DotProduct(const Vector& a, const Vector& b);
	extern inline float Distance(const Vector& a, const Vector& b);
	extern QAngle AngleBetweenXY(const Vector& a, const Vector& b);
	extern int Random(int min, int max);
	extern inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c);
	extern void VectorAngles(const Vector &forward, const Vector &pseudoup, QAngle &angles);
	extern void __fastcall VectorNormalizeFast(Vector& vec);
	extern void VectorAngles(const Vector& forward, QAngle &angles);
	extern void VectorAngles(const float *forward, float *angles);
	extern void ClampAngles(QAngle& angles);
	extern void CrossProduct(const Vector &v1, const Vector& v2, Vector& cross);
	extern QAngle CalcAngle(Vector src, Vector dst);
	extern void AngleVectors(const QAngle& angles, Vector *forward);
	extern void VectorTransform(Vector& in1, matrix3x4_t& in2, Vector &out);
	extern inline void NormalizeYaw(float &f);
	extern inline float NormalizeYawInPlace(float f);
	extern float NormalizeYawResolver(float value);
	extern void NormalizeAngles(QAngle& angles);
	extern float RandomFloat(float min, float max);
	extern void AngleVectorsNew(const QAngle &angles, Vector& forward, Vector& right, Vector& up);
	extern void NormalizeVector(Vector& vec);
	template<class T>
	const T& Clamp(const T& v, const T& lo, const T& hi)
	{
		return v < lo ? lo : v > hi ? hi : v;
	}
	extern unsigned int FindInDataMap(datamap_t *pMap, const char *name);
	extern float Lerp(float t, float a, float b);
	extern Vector Lerp(float t, const Vector& a, const Vector& b);
	extern Vector ExtrapolateInterpolatedVarType(const Vector &oldVal, const Vector &newVal, float divisor, float flExtrapolationAmount);
	extern int RandomSeed(int iSeed);
	extern int RandomInt(int iMinVal, int iMaxVal);
	extern void AngleIMatrix(const QAngle& angles, matrix3x4_t& matrix);
};
