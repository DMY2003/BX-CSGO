#include "Math.h"

namespace Math
{
	inline void SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);

		/*_asm
		{
			fld		DWORD PTR[radians]
			fsincos

			mov edx, DWORD PTR[cosine]
			mov eax, DWORD PTR[sine]

			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
		}*/
	}

	void AngleVectors(QAngle q, Vector& vec)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(q[1]), &sy, &cy);
		SinCos(DEG2RAD(q[0]), &sp, &cp);

		vec.x = cp*cy;
		vec.y = cp*sy;
		vec.z = -sp;
	}
	void AngleVectorr(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr*-sy);
		up.y = (cr * sp * sy + -sr*cy);
		up.z = (cr * cp);
	}
	void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}
	void ClampAngles(QAngle& angles)
	{
	if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = 89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = 180.0f;

		angles.z = 0;
	}
	void VectorAngles(const float *forward, float *angles)
	{
		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}


	void VectorAngles(const Vector& forward, QAngle &angles)
	{

		/*if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles[1] = 0.0f;
		}
		else
		{
			float len2d = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);

			angles[0] = RAD2DEG(atan2f(-forward[2], len2d));
			angles[1] = RAD2DEG(atan2f(forward[1], forward[0]));

			if (angles[0] < 0.0f)
				angles[0] += 360.0f;
			if (angles[1] < 0.0f)
				angles[1] += 360.0f;
		}*/

		float	tmp, yaw, pitch;

		if (forward[1] == 0 && forward[0] == 0)
		{
			yaw = 0;
			if (forward[2] > 0)
				pitch = 270;
			else
				pitch = 90;
		}
		else
		{
			yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
			if (yaw < 0)
				yaw += 360;

			tmp = sqrtf(forward[0] * forward[0] + forward[1] * forward[1]);
			pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
		
		/*if (forward[1] == 0.0f && forward[0] == 0.0f)
		{
			angles[0] = (forward[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
			angles[1] = 0.0f;  //yaw left/right
		}
		else
		{
			angles[0] = atan2(-forward[2], forward.Length2D()) * -180 / M_PI;
			angles[1] = atan2(forward[1], forward[0]) * 180 / M_PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;*/
	}

	//-----------------------------------------------------------------------------
	// Forward direction vector with a reference up vector -> Euler angles
	//-----------------------------------------------------------------------------
	Vector CrossProducts(const Vector &a, const Vector &b)
	{
		return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	void VectorAngles(const Vector &forward, const Vector &pseudoup, QAngle &angles)
	{
		Vector left = CrossProducts(pseudoup, forward);
		left.NormalizeInPlace();

		float forwardDist = forward.Length2D();

		if (forwardDist > 0.001f)
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(forward.y, forward.x) * 180 / 3.14159265358979323846f;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles.z = atan2f(left.z, upZ) * 180 / 3.14159265358979323846f;
		}
		else
		{
			angles.x = atan2f(-forward.z, forwardDist) * 180 / 3.14159265358979323846f;
			angles.y = atan2f(-left.x, left.y) * 180 / 3.14159265358979323846f;
			angles.z = 0;
		}
	}

	void __fastcall VectorNormalizeFast(Vector& vec)
	{
		// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
		float iradius = 1.f / (sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z) + FLT_EPSILON);

		vec.x *= iradius;
		vec.y *= iradius;
		vec.z *= iradius;
	}

	inline float DotProduct(const Vector& a, const Vector& b)
	{
		return(a.x*b.x + a.y*b.y + a.z*b.z);
	}

	inline float Distance(const Vector& a, const Vector& b)
	{
		return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y) *(a.y - b.y) + (a.z - b.z) *(a.z - b.z));
	}

	QAngle AngleBetweenXY(const Vector& a, const Vector& b)
	{
		QAngle angles;

		float distanceX = (b.x - a.x);
		float distanceY = (b.y - a.y);
		float distanceZ = (b.z - a.z);

		float hipotenuseXY = sqrtf(distanceX*distanceX + distanceY*distanceY);
		//float hipotenuseXZ = sqrt(distanceX*distanceX + distanceZ*distanceZ);
		//float hipotenuseYZ = sqrt(distanceY*distanceY + distanceZ*distanceZ);

		angles.x = atanf(distanceZ / hipotenuseXY) * (180.0f / M_PI_F) * -1.0f;
		angles.y = atanf(distanceY / distanceX) * (180.0f / M_PI_F);

		if (distanceX < 0)
			angles.y += 180.0f;

		angles.z = 0;

		return angles;
	}

	void CrossProduct(const Vector &v1, const Vector& v2, Vector& cross)
	{
		cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
		cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
		cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
	}

	int Random(int min, int max)
	{
		if (min == max)
			return min;

		if (max % 2 == 0)
			return min + (rand() % (max - min));

		return min + (rand() % (max - min + 1));
	}

	inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
	{
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	QAngle CalcAngle(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = src - dst;

		if (delta[1] == 0.0f && delta[0] == 0.0f)
		{
			angles[0] = (delta[2] > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
			angles[1] = 0.0f;  //yaw left/right
		}
		else
		{
			angles[0] = atan2(-delta[2], delta.Length2D()) * -180 / M_PI;
			angles[1] = atan2(delta[1], delta[0]) * 180 / M_PI;

			if (angles[1] > 90) angles[1] -= 180;
			else if (angles[1] < 90) angles[1] += 180;
			else if (angles[1] == 90) angles[1] = 0;
		}

		angles[2] = 0.0f;
		angles.NormalizeAngles();

		return angles;
	}

	void AngleVectors(const QAngle &angles, Vector *forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);

		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	void VectorTransform(Vector& in1, matrix3x4_t& in2, Vector &out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}

	void NormalizeYaw(float &f)
	{
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;

		while (f < -180.0f) f += 360.0f;
		while (f > 180.0f) f -= 360.0f;
	}
	void NormalizeAngles(QAngle& angles)
	{
		for (auto i = 0; i < 3; i++) {
			while (angles[i] < -180.0f) angles[i] += 360.0f;
			while (angles[i] >  180.0f) angles[i] -= 360.0f;
		}
	}
	float NormalizeYawResolver(float value)
	{
		while (value > 180)
			value -= 360.f;

		while (value < -180)
			value += 360.f;
		return value;
	}
	float NormalizeYawInPlace(float f)
	{
		if (std::isnan(f) || std::isinf(f))
			f = 0;

		if (f > 9999999)
			f = 0;

		if (f < -9999999)
			f = 0;
		
		while (f < -180.0f) f += 360.0f;
		while (f > 180.0f) f -= 360.0f;

		return f;
	}
	void NormalizeVector(Vector& vec)
	{
		for (auto i = 0; i < 3; i++) {
			while (vec[i] < -180.0f) vec[i] += 360.0f;
			while (vec[i] >  180.0f) vec[i] -= 360.0f;
		}
		vec[2] = 0.f;
	}
	void AngleVectorsNew(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr*-sy);
		up.y = (cr * sp * sy + -sr*cy);
		up.z = (cr * cp);
	}
	float RandomFloat(float min, float max)
	{
		typedef float(*RandomFloat_t)(float, float);
		static RandomFloat_t m_RandomFloat = NULL;

		if (!m_RandomFloat)
			m_RandomFloat = (RandomFloat_t)GetProcAddress(GetModuleHandle(xorstr("vstdlib.dll")), xorstr("RandomFloat"));

		return m_RandomFloat(min, max);
	}
	unsigned int FindInDataMap(datamap_t *pMap, const char *name)
	{
		while (pMap)
		{
			for (int i = 0; i<pMap->dataNumFields; i++)
			{
				if (pMap->dataDesc[i].fieldName == NULL)
					continue;

				if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
					return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (pMap->dataDesc[i].fieldType == FIELD_EMBEDDED)
				{
					if (pMap->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindInDataMap(pMap->dataDesc[i].td, name)) != 0)
							return offset;
					}
				}
			}
			pMap = pMap->baseMap;
		}

		return 0;
	}
	float Lerp(float t, float a, float b)
	{
		return a + (b - a) * t;
	}

	Vector Lerp(float t, const Vector& a, const Vector& b)
	{
		return a + (b - a) * t;
	}

	Vector ExtrapolateInterpolatedVarType(const Vector &oldVal, const Vector &newVal, float divisor, float flExtrapolationAmount)
	{
		return Lerp(1.0f + flExtrapolationAmount * divisor, oldVal, newVal);
	}

	int RandomSeed(int iSeed)
	{
		typedef int(__cdecl *RandomSeedFn)(int iSeed);

		static RandomSeedFn oRandomSeed = NULL;

		if (!oRandomSeed)
			oRandomSeed = (RandomSeedFn)GetProcAddress(GetModuleHandle(xorstr("vstdlib.dll")), xorstr("RandomSeed"));

		return oRandomSeed(iSeed);
	}

	int RandomInt(int iMinVal, int iMaxVal)
	{
		typedef int(__cdecl *RandomIntFn)(int iMinVal, int iMaxVal);

		static RandomIntFn oRandomInt = NULL;

		if (!oRandomInt)
			oRandomInt = (RandomIntFn)GetProcAddress(GetModuleHandle(xorstr("vstdlib.dll")), xorstr("RandomInt"));

		return oRandomInt(iMinVal, iMaxVal);
	}
	

	void AngleIMatrix(const QAngle& angles, matrix3x4_t& matrix)
	{
		float		sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		// matrix = (YAW * PITCH) * ROLL
		matrix[0][0] = cp*cy;
		matrix[0][1] = cp*sy;
		matrix[0][2] = -sp;
		matrix[1][0] = sr*sp*cy + cr*-sy;
		matrix[1][1] = sr*sp*sy + cr*cy;
		matrix[1][2] = sr*cp;
		matrix[2][0] = (cr*sp*cy + -sr*-sy);
		matrix[2][1] = (cr*sp*sy + -sr*cy);
		matrix[2][2] = cr*cp;
		matrix[0][3] = 0.f;
		matrix[1][3] = 0.f;
		matrix[2][3] = 0.f;
	}
}
