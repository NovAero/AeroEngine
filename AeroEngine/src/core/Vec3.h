#pragma once

namespace AE::Math {

	class Vec3 {
	public:

		Vec3();
		Vec3(const float x, const float y, const float z);
		Vec3(const float value);

	public:

		float& operator[](int index);
		const float& operator[](int index) const;

		Vec3 operator+=(const Vec3 rhs);
		Vec3 operator-=(const Vec3 rhs);
		Vec3 operator*=(float scale);
		Vec3 operator/=(float scale);

		//Const cast operator
		operator float* () const;

		float Dot(const Vec3& rhs);

		Vec3& Cross(const Vec3& rhs);

		float Magnitude() const;
		float MagnitudeSqr() const;

		Vec3& Normalise();
		[[nodiscard]] Vec3 Normalised() const;

	public:

		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
			float data[3];
		};
	};

	float Dot(Vec3 lhs, Vec3 rhs);
	Vec3 Cross(Vec3 lhs, Vec3 rhs);
	Vec3 V3Distance(Vec3 lhs, Vec3 rhs);
	float V3DistanceF(Vec3 lhs, Vec3 rhs);
	float V3DistanceSqrF(Vec3 lhs, Vec3 rhs);

	Vec3 operator*(Vec3 v, float s);
	Vec3 operator*(float s, Vec3 v);
	Vec3 operator/(Vec3 v, float s);
	Vec3 operator+(Vec3 lhs, Vec3 rhs);
	Vec3 operator-(Vec3 lhs, Vec3 rhs);
	Vec3 operator-(Vec3 v);
}