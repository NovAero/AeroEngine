#include "AEPCH.h"
#include "Vec2.h"

namespace AE::Math {

	Vec2::Vec2(const float value) : x(value), y(value) {}

	Vec2::Vec2(const float X, const float Y) : x(X), y(Y) {}

	Vec2::Vec2(const Vec2& other) {
		*this = other;
	}

	void Vec2::operator=(const Vec2& other)
	{
		data[0] = other.data[0];
		data[1] = other.data[1];
	}

	float& Vec2::operator[](int index)
	{
		if (index < 0 || index > 1) index = 0;
		return data[index];
	}

	const float& Vec2::operator[](int index) const
	{
		if (index < 0 || index > 1) index = 0;
		return data[index];
	}

	Vec2& Vec2::operator+=(Vec2 v)
	{
		x += v.x;
		y += v.y;

		return *this;
	}

	Vec2& Vec2::operator-=(Vec2 v)
	{
		x -= v.x;
		y -= v.y;

		return *this;
	}

	Vec2& Vec2::operator*=(float s)
	{
		x *= s;
		y *= s;

		return *this;
	}

	Vec2& Vec2::operator/=(float s)
	{
		x /= s;
		y /= s;

		return *this;
	}

	float Vec2::Dot(const Vec2& rhs)
	{
		return ((x * rhs.x) + (y * rhs.y));
	}

	float Vec2::Magnitude() const
	{
		return sqrtf(x * x + y * y);
	}

	void Vec2::Magnitude(float mag)
	{
		Normalise();
		*this *= mag;
	}

	float Vec2::MagnitudeSqr() const
	{
		return x * x + y * y;
	}

	Vec2& Vec2::Normalise() {
		float mag = Magnitude();
		if (mag > 0.0f) *this /= mag;
		return *this;
	}

	Vec2 Vec2::Normalised() const
	{
		float mag = Magnitude();
		if (mag > 0.0f) return *this / mag;
		else return *this;
	}

	Vec2& Vec2::RotateBy90()
	{
		float s = x;
		x = -y;
		y = s;
		return *this;
	}

	Vec2& Vec2::RotateBy270()
	{
		float s = x;
		x = y;
		y = -s;
		return *this;
	}

	Vec2& Vec2::RotateBy(float angle)
	{
		float cosAngle = cosf(angle);
		float sinAngle = sinf(angle);
		float oldX = x;
		x = x * cosAngle - y * sinAngle;
		y = oldX * sinAngle + y * cosAngle;
		return *this;
	}

	Vec2& Vec2::RotateBy(float cosAngle, float sinAngle)
	{
		//For when you have the cos and sine already, and need to rotate the same amount
		//a lot - avoid recalculating the trig
		float oldX = x;
		x = x * cosAngle - y * sinAngle;
		y = oldX * sinAngle + y * cosAngle;
		return *this;
	}

	Vec2 Vec2::GetRotatedBy90() const
	{
		return Vec2(-y, x);
	}

	Vec2 Vec2::GetRotatedBy270() const
	{
		return Vec2(y, -x);
	}

	Vec2 Vec2::GetRotatedBy(float angle) const
	{
		float cosAngle = cosf(angle);
		float sinAngle = sinf(angle);
		return Vec2(
			x * cosAngle - y * sinAngle,
			x * sinAngle + y * cosAngle
		);
	}

	Vec2 Vec2::GetRotatedBy(float cosAngle, float sinAngle) const
	{
		return Vec2(
			x * cosAngle - y * sinAngle,
			x * sinAngle + y * cosAngle
		);
	}

	Vec2::operator float* () const
	{
		return const_cast<float*>(data);
	}


	Vec2 operator+(Vec2 lhs, Vec2 rhs)
	{
		return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
	}

	Vec2 operator-(Vec2 lhs, Vec2 rhs)
	{
		return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
	}

	Vec2 operator*(Vec2 v, float s)
	{
		return Vec2(v.x * s, v.y * s);
	}

	Vec2 operator*(float s, Vec2 v)
	{
		return Vec2(v.x * s, v.y * s);
	}

	Vec2 operator/(Vec2 v, float s)
	{
		return Vec2(v.x / s, v.y / s);
	}

	//Negate vector operator (-vec)
	Vec2 operator-(Vec2 v)
	{
		return Vec2(-v.x, -v.y);
	}

	float Dot(Vec2 a, Vec2 b)
	{
		return (a.Dot(b));
	}

	float V2Cross(Vec2 a, Vec2 b)
	{
		//Cross isnt real in 2D space, returns the pseudo z axis
		//Useful for stuff like checking left or right turn

		return a.x * b.y - a.y * b.x;
	}

	float AngleBetween(Vec2 a, Vec2 b)
	{
		return acosf(Dot(a.Normalised(), b.Normalised()));
	}

}