#pragma once
#include "Util.h"
class QAngles
{

public:

	QAngles(void)
	{
		Init();
	}

	QAngles(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

	QAngles(const float* clr)
	{
		Init(clr[0], clr[1], clr[2]);
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		pitch = ix;
		yaw = iy;
		roll = iz;
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	float &operator[](int i)
	{
		return ((float*)this)[i];
	}

	bool operator==(const QAngles &src) const
	{
		return (src.pitch == pitch) && (src.yaw == yaw) && (src.roll == roll);
	}

	bool operator!=(const QAngles &src) const
	{
		return (src.pitch != pitch) || (src.yaw != yaw) || (src.roll != roll);
	}

	QAngles &operator+=(const QAngles &v)
	{
		pitch += v.pitch; yaw += v.yaw; roll += v.roll;
		return *this;
	}

	QAngles &operator-=(const QAngles &v)
	{
		pitch -= v.pitch; yaw -= v.yaw; roll -= v.roll;
		return *this;
	}

	QAngles &operator*=(float fl)
	{
		pitch *= fl;
		yaw *= fl;
		roll *= fl;
		return *this;
	}

	QAngles &operator*=(const QAngles &v)
	{
		pitch *= v.pitch;
		yaw *= v.yaw;
		roll *= v.roll;
		return *this;
	}

	QAngles &operator/=(const QAngles &v)
	{
		pitch /= v.pitch;
		yaw /= v.yaw;
		roll /= v.roll;
		return *this;
	}

	QAngles &operator+=(float fl)
	{
		pitch += fl;
		yaw += fl;
		roll += fl;
		return *this;
	}

	QAngles &operator/=(float fl)
	{
		pitch /= fl;
		yaw /= fl;
		roll /= fl;
		return *this;
	}

	QAngles &operator-=(float fl)
	{
		pitch -= fl;
		yaw -= fl;
		roll -= fl;
		return *this;
	}

	QAngles &operator=(const QAngles &vOther)
	{
		pitch = vOther.pitch; yaw = vOther.yaw; roll = vOther.roll;
		return *this;
	}

	QAngles operator-(void) const
	{
		return QAngles(-pitch, -yaw, -roll);
	}

	QAngles operator+(const QAngles &v) const
	{
		return QAngles(pitch + v.pitch, yaw + v.yaw, roll + v.roll);
	}

	QAngles operator-(const QAngles &v) const
	{
		return QAngles(pitch - v.pitch, yaw - v.yaw, roll - v.roll);
	}

	QAngles operator*(float fl) const
	{
		return QAngles(pitch * fl, yaw * fl, roll * fl);
	}

	QAngles operator*(const QAngles &v) const
	{
		return QAngles(pitch * v.pitch, yaw * v.yaw, roll * v.roll);
	}

	QAngles operator/(float fl) const
	{
		return QAngles(pitch / fl, yaw / fl, roll / fl);
	}

	QAngles operator/(const QAngle &v) const
	{
		return QAngles(pitch / v.x, yaw / v.y, roll / v.z);
	}

	float Length() const
	{
		return sqrt(pitch*pitch + yaw*yaw + roll*roll);
	}

	float LengthSqr(void) const
	{
		return (pitch*pitch + yaw*yaw + roll*roll);
	}

	bool IsZero(float tolerance = 0.01f) const
	{
		return (pitch > -tolerance && pitch < tolerance &&
			yaw > -tolerance && yaw < tolerance &&
			roll > -tolerance && roll < tolerance);
	}

	auto Clamp()
	{
		while (this->pitch < -89.0f)
			this->pitch += 89.0f;

		if (this->pitch > 89.0f)
			this->pitch = 89.0f;

		while (this->yaw < -180.0f)
			this->yaw += 360.0f;

		while (this->yaw > 180.0f)
			this->yaw -= 360.0f;

		this->roll = 0.0f;
	}

	auto Normalize()
	{
		auto x_rev = this->pitch / 360.f;
		if (this->pitch > 180.f || this->pitch < -180.f)
		{
			x_rev = abs(x_rev);
			x_rev = round(x_rev);

			if (this->pitch < 0.f)
				this->pitch = (this->pitch + 360.f * x_rev);

			else
				this->pitch = (this->pitch - 360.f * x_rev);
		}

		auto y_rev = this->yaw / 360.f;
		if (this->yaw > 180.f || this->yaw < -180.f)
		{
			y_rev = abs(y_rev);
			y_rev = round(y_rev);

			if (this->yaw < 0.f)
				this->yaw = (this->yaw + 360.f * y_rev);

			else
				this->yaw = (this->yaw - 360.f * y_rev);
		}

		auto z_rev = this->roll / 360.f;
		if (this->roll > 180.f || this->roll < -180.f)
		{
			z_rev = abs(z_rev);
			z_rev = round(z_rev);

			if (this->roll < 0.f)
				this->roll = (this->roll + 360.f * z_rev);

			else
				this->roll = (this->roll - 360.f * z_rev);
		}
	}

	float RealYawDifference(QAngles dst, QAngles add = QAngles());
	float Difference(QAngles dst, QAngles add);
	void SinCos(float a, float* s, float*c);
	void AngleVectors(const QAngles &angles, Vector& forward);
	QAngles NormalizeYaw()
	{
		while (this->yaw > 180)
			this->yaw -= 360;

		while (this->yaw < -180)
			this->yaw += 360;

		return *this;
	}

	float
		pitch,
		yaw,
		roll;
};

inline QAngles operator*(float lhs, const QAngles &rhs)
{
	return rhs * lhs;
}

inline QAngles operator/(float lhs, const QAngles &rhs)
{
	return rhs / lhs;
}