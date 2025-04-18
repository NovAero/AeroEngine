#pragma once

namespace AE::Math {
	class Vec2 {
	public:
		Vec2() = default;
		Vec2(const float value);
		Vec2(const float X, const float Y);
		Vec2(const Vec2& other);
		~Vec2() = default;

		void operator=(const Vec2& other);

		//access operators for matrix usage
		float& operator[](int index);
		const float& operator[](int index) const;

		Vec2& operator+=(Vec2 v);
		Vec2& operator-=(Vec2 v);
		Vec2& operator*=(float s);
		Vec2& operator/=(float s);

		float Dot(const Vec2& rhs);

		float Magnitude() const;
		void Magnitude(float mag);
		
		float MagnitudeSqr() const;

		Vec2& Normalise();
		[[nodiscard]] Vec2 Normalised() const;

		Vec2& RotateBy90();
		Vec2& RotateBy270();
		Vec2& RotateBy(float angle);
		Vec2& RotateBy(float cosAngle, float sinAngle);

		[[nodiscard]] Vec2 GetRotatedBy90() const;
		[[nodiscard]] Vec2 GetRotatedBy270() const;
		[[nodiscard]] Vec2 GetRotatedBy(float angle) const;
		[[nodiscard]] Vec2 GetRotatedBy(float cosAngle, float sinAngle) const;

		//Const cast operator
		operator float*() const;

	public:
		union 
		{
			struct { float x, y; };
			float data[2];
		};

	};

	Vec2 operator+(Vec2 lhs, Vec2 rhs);
	Vec2 operator-(Vec2 lhs, Vec2 rhs);
	Vec2 operator*(Vec2 v, float s);
	Vec2 operator*(float s, Vec2 v);
	Vec2 operator/(Vec2 v, float s);
	Vec2 operator-(Vec2 v);

	float Dot(Vec2 a, Vec2 b);
	float V2Cross(Vec2 a, Vec2 b);
	float AngleBetween(Vec2 a, Vec2 b);
}