#include "AEPCH.h"
#include "Vec3.h"

namespace AE::Math {
	Vec3::Vec3() : Vec3(0)
	{
	}
	Vec3::Vec3(const float x, const float y, const float z) : x(x), y(y), z(z)
	{
	}
	Vec3::Vec3(const float value) : x(value), y(value), z(value)
	{
	}

	float& Vec3::operator[](int index)
	{
		if (index < 0 || index > 2) index = 0;
		return data[index];
	}

	const float& Vec3::operator[](int index) const
	{
		if (index < 0 || index > 2) index = 0;
		return data[index];
	}

	Vec3 Vec3::operator+=(const Vec3 rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3 Vec3::operator-=(const Vec3 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3 Vec3::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vec3 Vec3::operator/=(float scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}

	Vec3::operator float* () const
	{
		return const_cast<float*>(data);
	}

	float Vec3::Dot(const Vec3& rhs)
	{
		return AE::Math::Dot(*this, rhs);
	}

	Vec3& Vec3::Cross(const Vec3& rhs)
	{
		data[0] = (y * rhs.z) - (z * rhs.y);
		data[1] = (z * rhs.x) - (x * rhs.z);
		data[2] = (x * rhs.y) - (y * rhs.x);
		return *this;
	}

	float Vec3::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float Vec3::MagnitudeSqr() const
	{
		return x * x + y * y + z * z;
	}

	Vec3& Vec3::Normalise()
	{
		float mag = Magnitude();
		if (mag > 0.f) *this /= mag;
		return *this;
	}

	Vec3 Vec3::Normalised() const
	{
		float mag = Magnitude();
		if (mag > 0.0f) return *this / mag;
		else return *this;
	}

	float Dot(Vec3 lhs, Vec3 rhs)
	{
		return (lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z);
	}
	Vec3 Cross(Vec3 lhs, Vec3 rhs)
	{
		return Vec3(
			(lhs.y * rhs.z) - (lhs.z * rhs.y),
			(lhs.z * rhs.x) - (lhs.x * rhs.z),
			(lhs.x * rhs.y) - (lhs.y * rhs.x)
		);
	}

	Vec3 V3Distance(Vec3 lhs, Vec3 rhs)
	{
		return lhs - rhs;
	}

	float V3DistanceF(Vec3 lhs, Vec3 rhs)
	{
		return V3Distance(lhs, rhs).Magnitude();
	}

	float V3DistanceSqrF(Vec3 lhs, Vec3 rhs)
	{
		return V3Distance(lhs, rhs).MagnitudeSqr();
	}

	Vec3 operator*(Vec3 v, float s)
	{
		return Vec3(v.x * s, v.y * s, v.z * s);
	}
	Vec3 operator*(float s, Vec3 v)
	{
		return Vec3(v.x * s, v.y * s, v.z * s);
	}
	Vec3 operator/(Vec3 v, float s)
	{
		return Vec3(v.x / s, v.y / s, v.z / s);
	}
	Vec3 operator+(Vec3 lhs, Vec3 rhs)
	{
		return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
	}
	Vec3 operator-(Vec3 lhs, Vec3 rhs)
	{
		return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
	}
	Vec3 operator-(Vec3 v)
	{
		return Vec3(-v.x, -v.y, -v.z);
	}
}