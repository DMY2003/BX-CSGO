#pragma once

#include <limits>

typedef float vec_t;

inline unsigned long& FloatBits(vec_t& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline unsigned long const& FloatBits(vec_t const& f)
{
	return *reinterpret_cast<unsigned long const*>(&f);
}

inline vec_t BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<vec_t*>(&i);
}

inline bool IsFinite(vec_t f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline unsigned long FloatAbsBits(vec_t f)
{
	return FloatBits(f) & 0x7FFFFFFF;
}

#define FLOAT32_NAN_BITS     (unsigned long)0x7FC00000	// not a number!
#define FLOAT32_NAN          BitsToFloat( FLOAT32_NAN_BITS )

#define VEC_T_NAN FLOAT32_NAN


class Vector2D
{
public:
	float x, y;

	Vector2D() {}
	Vector2D(float x_, float y_) { x = x_; y = y_; }
};

class Vector {
public:
	float x, y, z;
	Vector(void);
	Vector(float X, float Y, float Z);
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	Vector& Clamp();
	Vector& NormalizeAngles();
	Vector Cross(const Vector &vOther);
	float NormalizeMovement() const;
	inline float LengthSqr() {
		return (x*x + y*y + z*z);
	}
	inline float Length() {
		return sqrtf(LengthSqr());
	}
	__inline bool IsValid() const
	{
		return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
	}
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	__forceinline Vector&	operator+=(const Vector &v);
	__forceinline Vector&	operator-=(const Vector &v);
	__forceinline Vector&	operator*=(const Vector &v);
	__forceinline Vector&	operator*=(float s);
	__forceinline Vector&	operator/=(const Vector &v);
	__forceinline Vector&	operator/=(float s);
	__forceinline Vector&	operator+=(float fl);
	__forceinline Vector&	operator-=(float fl);
	Vector& operator=(const Vector &vOther);
	Vector	operator-(const Vector& v) const;
	Vector	operator+(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;
	inline Vector operator+(float fl) const;
	inline Vector operator-(float fl) const;
	inline Vector GetNormalized(void);
	//inline void Normalize(void);
	inline Vector Normalize(void);
	float Dot(const Vector& vOther) const;
	float Dot(const float* fOther) const;
	float Length2D(void) const;
	float NormalizeInPlace();
	inline Vector Normalized() const;
};

typedef Vector QAngle;

inline Vector& Vector::Clamp() {

	return *this;
}
inline Vector Vector::Normalized() const
{
	Vector res = *this;

	return res;
}
inline Vector Vector::operator-(float fl) const
{
	return Vector(x - fl, y - fl, z - fl);
}
inline Vector Vector::operator+(float fl) const
{
	return Vector(x + fl, y + fl, z + fl);
}
inline void VectorCrossProduct(const Vector &a, const Vector &b, Vector &result)
{
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
}
inline float Vector::NormalizeMovement() const
{
	Vector res = *this;
	float l = res.Length();
	if (l != 0.0f)
	{
		res /= l;
	}
	else
	{
		res.x = res.y = res.z = 0.0f;
	}
	return l;
}
inline Vector Vector::Cross(const Vector &vOther)
{
	Vector res;
	VectorCrossProduct(*this, vOther, res);
	return res;
}
inline Vector& Vector::NormalizeAngles()
{
	return *this;
}

inline void Vector::Init(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}

inline Vector::Vector(float X, float Y, float Z) {
	x = X; y = Y; z = Z;
}

inline Vector::Vector(void) {}

inline void Vector::Zero() {
	x = y = z = 0.0f;
}

inline void VectorClear(Vector& a) {
	a.x = a.y = a.z = 0.0f;
}

inline Vector& Vector::operator=(const Vector &vOther) {
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

inline float& Vector::operator[](int i) {
	return ((float*)this)[i];
}

inline float Vector::operator[](int i) const {
	return ((float*)this)[i];
}

inline bool Vector::operator==(const Vector& src) const {
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector::operator!=(const Vector& src) const {
	return (src.x != x) || (src.y != y) || (src.z != z);
}

__forceinline  Vector& Vector::operator+=(const Vector& v) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}

__forceinline  Vector& Vector::operator-=(const Vector& v) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

__forceinline  Vector& Vector::operator*=(float fl) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

__forceinline  Vector& Vector::operator*=(const Vector& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

__forceinline Vector&	Vector::operator+=(float fl) {
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

__forceinline Vector&	Vector::operator-=(float fl) {
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}

__forceinline  Vector& Vector::operator/=(float fl) {
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

__forceinline  Vector& Vector::operator/=(const Vector& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vector Vector::operator+(const Vector& v) const {
	Vector res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}


inline Vector Vector::operator-(const Vector& v) const {
	Vector res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline Vector Vector::operator*(float fl) const {
	Vector res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline Vector Vector::operator*(const Vector& v) const {
	Vector res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

inline Vector Vector::operator/(float fl) const {
	Vector res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline Vector Vector::operator/(const Vector& v) const {
	Vector res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}


class matrix3x4_t
{
public:
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	void Init(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	//-----------------------------------------------------------------------------
	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	//-----------------------------------------------------------------------------
	matrix3x4_t(const Vector& xAxis, const Vector& yAxis, const Vector& zAxis, const Vector &vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}

	inline void SetOrigin(Vector const & p)
	{
		m_flMatVal[0][3] = p.x;
		m_flMatVal[1][3] = p.y;
		m_flMatVal[2][3] = p.z;
	}

	inline void Invalidate(void)
	{
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 4; j++) {
				m_flMatVal[i][j] = std::numeric_limits<float>::infinity();;
			}
		}
	}
	Vector at(int i) const { return Vector{ m_flMatVal[0][i], m_flMatVal[1][i], m_flMatVal[2][i] }; }
	float *operator[](int i) { return m_flMatVal[i]; }
	const float *operator[](int i) const { return m_flMatVal[i]; }
	float *Base() { return &m_flMatVal[0][0]; }
	const float *Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

inline Vector Vector::GetNormalized(void) {
	float length = this->Length();

	if (length != 0)
		return Vector(*this / this->Length());

	return Vector(0, 0, 1);
}
//inline void Vector::Normalize(void)
inline Vector Vector::Normalize(void)
{
	*this = GetNormalized();

	/*Vector vector;
	float length = this->Length();

	if (length != 0) {
		vector.x = x / length;
		vector.y = y / length;
		vector.z = z / length;
	}
	else
		vector.x = vector.y = 0.0f; vector.z = 1.0f;
		
	return vector;*/

	return *this;
}
inline float Vector::Dot(const Vector& vOther) const
{
	const Vector& a = *this;

	return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}
inline float Vector::Dot(const float* fOther) const
{
	const Vector& a = *this;

	return(a.x*fOther[0] + a.y*fOther[1] + a.z*fOther[2]);
}
inline float Vector::Length2D(void) const
{
	float root = 0.0f;

	float sqst = x*x + y*y;

	__asm
	{
		sqrtss xmm0, sqst
		movss root, xmm0
	}

	return root;
}
inline float Vector::NormalizeInPlace()
{
	Vector& v = *this;

	float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

	v.x *= iradius;
	v.y *= iradius;
	v.z *= iradius;

	return 1;
}


class RadianEuler
{
public:
	inline RadianEuler(void) { }
	inline RadianEuler(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	//inline RadianEuler(Quaternion const &q);	// evil auto type promotion!!!
	//inline RadianEuler(QAngle const &angles);	// evil auto type promotion!!!

												// Initialization
	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) { x = ix; y = iy; z = iz; }

	//	conversion to qangle
	//QAngle ToQAngle(void) const;
	//bool IsValid() const;
	//void Invalidate();

	inline float *Base() { return &x; }
	inline const float *Base() const { return &x; }

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};


class Quaternion				// same data-layout as engine's vec4_t,
{								//		which is a vec_t[4]
public:
	inline Quaternion(vec_t ix, vec_t iy, vec_t iz, vec_t iw) : x(ix), y(iy), z(iz), w(iw) { }
	//inline Quaternion(RadianEuler const &angle);	// evil auto type promotion!!!

	inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

	inline Quaternion() { }

	//bool IsValid() const;
	//void Invalidate();

	bool operator==(const Quaternion &src) {
		return (x == src.x) && (y == src.y) && (z == src.z) && (w == src.w);
	}

	bool operator!=(const Quaternion &src) {
		return !operator==(src);
	}

	vec_t* Base() { return (vec_t*)this; }
	const vec_t* Base() const { return (vec_t*)this; }

	// array access...
	vec_t operator[](int i) { return ((vec_t*)this)[i]; }
	//vec_t& operator[](int i) { return ((vec_t*)this)[i]; }

	vec_t x, y, z, w;
};



//=========================================================
// 4D Vector4D
//=========================================================

class Vector4D
{
public:
	// Members
	vec_t x, y, z, w;

	// Construction/destruction
	Vector4D(void);
	Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W);
	Vector4D(const float *pFloat);

	// Initialization
	void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f, vec_t iw = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	vec_t operator[](int i) const;
	vec_t& operator[](int i);

	// Base address...
	inline vec_t* Base();
	inline vec_t const* Base() const;

	// Cast to Vector and Vector2D...
	Vector& AsVector3D();
	Vector const& AsVector3D() const;

	// Initialization methods
	void Random(vec_t minVal, vec_t maxVal);

	// equality
	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	// arithmetic operations
	Vector4D&	operator+=(const Vector4D &v);
	Vector4D&	operator-=(const Vector4D &v);
	Vector4D&	operator*=(const Vector4D &v);
	Vector4D&	operator*=(float s);
	Vector4D&	operator/=(const Vector4D &v);
	Vector4D&	operator/=(float s);

	// negate the Vector4D components
	void	Negate();

	// Get the Vector4D's magnitude.
	vec_t	Length() const;

	// Get the Vector4D's magnitude squared.
	vec_t	LengthSqr(void) const;

	// return true if this vector is (0,0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance &&
			w > -tolerance && w < tolerance);
	}

	// Get the distance from this Vector4D to the other one.
	vec_t	DistTo(const Vector4D &vOther) const;

	// Get the distance from this Vector4D to the other one squared.
	vec_t	DistToSqr(const Vector4D &vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector4D equation (because it's done per-component
	// rather than per-Vector4D).
	void	MulAdd(Vector4D const& a, Vector4D const& b, float scalar);

	// Dot product.
	vec_t	Dot(Vector4D const& vOther) const;

	// No copy constructors allowed if we're in optimal mode
#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
#else
public:
#endif
	Vector4D(Vector4D const& vOther);

	// No assignment operators either...
	Vector4D& operator=(Vector4D const& src);
};

const Vector4D vec4_origin(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4D vec4_invalid(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

//-----------------------------------------------------------------------------
// Vector4D related operations
//-----------------------------------------------------------------------------

// Vector4D clear
void Vector4DClear(Vector4D& a);

// Copy
void Vector4DCopy(Vector4D const& src, Vector4D& dst);

// Vector4D arithmetic
void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, vec_t b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, vec_t b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result);


#define Vector4DExpand( v ) (v).x, (v).y, (v).z, (v).w

// Normalization
vec_t Vector4DNormalize(Vector4D& v);

// Length
vec_t Vector4DLength(Vector4D const& v);

// Dot Product
vec_t DotProduct4D(Vector4D const& a, Vector4D const& b);

// Linearly interpolate between two vectors
void Vector4DLerp(Vector4D const& src1, Vector4D const& src2, vec_t t, Vector4D& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector4D methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector4D::Vector4D(void)
{
#ifdef _DEBUG
	// Initialize to NAN to catch errors
	x = y = z = w = VEC_T_NAN;
#endif
}

inline Vector4D::Vector4D(vec_t X, vec_t Y, vec_t Z, vec_t W)
{
	x = X; y = Y; z = Z; w = W;
}

inline Vector4D::Vector4D(const float *pFloat)
{
	x = pFloat[0]; y = pFloat[1]; z = pFloat[2]; w = pFloat[3];
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline Vector4D::Vector4D(const Vector4D &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector4D::Init(vec_t ix, vec_t iy, vec_t iz, vec_t iw)
{
	x = ix; y = iy; z = iz;	w = iw;
}

inline void Vector4D::Random(vec_t minVal, vec_t maxVal)
{
	x = minVal + ((vec_t)rand() / 0x7fff) * (maxVal - minVal);
	y = minVal + ((vec_t)rand() / 0x7fff) * (maxVal - minVal);
	z = minVal + ((vec_t)rand() / 0x7fff) * (maxVal - minVal);
	w = minVal + ((vec_t)rand() / 0x7fff) * (maxVal - minVal);
}

inline void Vector4DClear(Vector4D& a)
{
	a.x = a.y = a.z = a.w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector4D& Vector4D::operator=(const Vector4D &vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline vec_t& Vector4D::operator[](int i)
{
	return ((vec_t*)this)[i];
}

inline vec_t Vector4D::operator[](int i) const
{
	return ((vec_t*)this)[i];
}

//-----------------------------------------------------------------------------
// Cast to Vector and Vector2D...
//-----------------------------------------------------------------------------

inline Vector& Vector4D::AsVector3D()
{
	return *(Vector*)this;
}

inline Vector const& Vector4D::AsVector3D() const
{
	return *(Vector const*)this;
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline vec_t* Vector4D::Base()
{
	return (vec_t*)this;
}

inline vec_t const* Vector4D::Base() const
{
	return (vec_t const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector4D::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z) && IsFinite(w);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector4D::operator==(Vector4D const& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

inline bool Vector4D::operator!=(Vector4D const& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector4DCopy(Vector4D const& src, Vector4D& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}

inline void	Vector4D::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y; rgfl[2] = z; rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void Vector4D::Negate()
{
	x = -x; y = -y; z = -z; w = -w;
}

inline Vector4D& Vector4D::operator+=(const Vector4D& v)
{
	x += v.x; y += v.y; z += v.z; w += v.w;
	return *this;
}

inline Vector4D& Vector4D::operator-=(const Vector4D& v)
{
	x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	return *this;
}

inline Vector4D& Vector4D::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	w *= fl;
	return *this;
}

inline Vector4D& Vector4D::operator*=(Vector4D const& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

inline Vector4D& Vector4D::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	w *= oofl;
	return *this;
}

inline Vector4D& Vector4D::operator/=(Vector4D const& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

inline void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
}

inline void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
}

inline void Vector4DMultiply(Vector4D const& a, vec_t b, Vector4D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	c.w = a.w * b;
}

inline void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
}

inline void Vector4DDivide(Vector4D const& a, vec_t b, Vector4D& c)
{
	vec_t oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
	c.w = a.w * oob;
}

inline void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
	c.w = a.w / b.w;
}

inline void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result)
{
	result.x = start.x + s*dir.x;
	result.y = start.y + s*dir.y;
	result.z = start.z + s*dir.z;
	result.w = start.w + s*dir.w;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector4D::MulAdd(Vector4D const& a, Vector4D const& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
	w = a.w + b.w * scalar;
}

inline void Vector4DLerp(const Vector4D& src1, const Vector4D& src2, vec_t t, Vector4D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
	dest[2] = src1[2] + (src2[2] - src1[2]) * t;
	dest[3] = src1[3] + (src2[3] - src1[3]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------

inline vec_t DotProduct4D(const Vector4D& a, const Vector4D& b)
{
	return(a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w);
}

// for backwards compatability
inline vec_t Vector4D::Dot(Vector4D const& vOther) const
{
	return DotProduct4D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline vec_t Vector4DLength(Vector4D const& v)
{
	return (vec_t)sqrtf(v.x*v.x + v.y*v.y + v.z*v.z + v.w*v.w);
}

inline vec_t Vector4D::LengthSqr(void) const
{
	return (x*x + y*y + z*z + w*w);
}

inline vec_t Vector4D::Length(void) const
{
	return Vector4DLength(*this);
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------

// FIXME: Can't use until we're un-macroed in mathlib.h
inline vec_t Vector4DNormalize(Vector4D& v)
{
	vec_t l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = v.z = v.w = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// Get the distance from this Vector4D to the other one 
//-----------------------------------------------------------------------------

inline vec_t Vector4D::DistTo(const Vector4D &vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline vec_t Vector4D::DistToSqr(const Vector4D &vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}

