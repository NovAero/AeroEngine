#include "AEPCH.h"
#include "Vec4.h"

namespace AE::Math {

	Vec4::Vec4() : Vec4(0.f)
	{
		w = 1.f;
	}

	Vec4::Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
	{
	}

	Vec4::Vec4(const float value) : x(value), y(value), z(value), w(value)
	{
	}

	float& Vec4::operator[](int index)
	{
		if (index < 0 || index > 3) index = 0;
		return data[index];
	}

	const float& Vec4::operator[](int index) const
	{
		if (index < 0 || index > 3) index = 0;
		return data[index];
	}

	Vec4 Vec4::operator+=(const Vec4 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vec4 Vec4::operator-=(const Vec4 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vec4 Vec4::operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}

	Vec4 Vec4::operator/=(float s)
	{
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}

	Vec4::operator float* () const
	{
		return const_cast<float*>(data);
	}

	float Vec4::Dot(const Vec4& rhs)
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}

	Vec4& Vec4::Cross(const Vec4& rhs)
	{
		x = (y * rhs.z) - (z * rhs.y);
		y = (z * rhs.x) - (x * rhs.z);
		z = (x * rhs.y) - (y * rhs.x);
		
		return *this;
	}

	float Vec4::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	float Vec4::MagnitudeSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	Vec4& Vec4::Normalise()
	{
		float mag = Magnitude();
		if (mag > 0.f) *this /= mag;
		return *this;
	}

	Vec4 Vec4::Normalised() const
	{
		float mag = Magnitude();
		if (mag > 0.0f) return *this / mag;
		else return *this;
	}

	float Dot(Vec4 lhs, Vec4 rhs)
	{
		return lhs.Dot(rhs);
	}

	Vec4 operator*(Vec4 v, float s)
	{
		return Vec4(v.x * s, v.y * s, v.z * s, v.w);
	}

	Vec4 operator*(float s, Vec4 v)
	{
		return Vec4(v.x * s, v.y * s, v.z * s, v.w);
	}

	Vec4 operator/(Vec4 v, float s)
	{
		return Vec4(v.x / s, v.y / s, v.z / s, v.w);
	}

	Vec4 operator+(Vec4 lhs, Vec4 rhs)
	{
		return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	Vec4 operator-(Vec4 lhs, Vec4 rhs)
	{
		return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	Vec4 operator-(Vec4 v)
	{
		return Vec4(-v.x, -v.y, -v.z, -v.w);
	}

}