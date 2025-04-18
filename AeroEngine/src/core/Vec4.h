#pragma once

namespace AE::Math {

	class Vec4 {
	public:

		Vec4();
		Vec4(const float x, const float y, const float z, const float w);
		Vec4(const float value);

	public:

		float& operator[](int index);
		const float& operator[](int index) const;

		Vec4 operator+=(const Vec4 rhs);
		Vec4 operator-=(const Vec4 rhs);
		Vec4 operator*=(float scale);
		Vec4 operator/=(float scale);

		//Const cast operator
		operator float* () const;

		float Dot(const Vec4& rhs);

		Vec4& Cross(const Vec4& rhs);

		float Magnitude() const;
		float MagnitudeSqr() const;

		Vec4& Normalise();
		[[nodiscard]] Vec4 Normalised() const;

	public:

		union {
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
			float data[4];
		};
	};

	float Dot(Vec4 lhs, Vec4 rhs);

	Vec4 operator*(Vec4 v, float s);
	Vec4 operator*(float s, Vec4 v);
	Vec4 operator/(Vec4 v, float s);
	Vec4 operator+(Vec4 lhs, Vec4 rhs);
	Vec4 operator-(Vec4 lhs, Vec4 rhs);
	Vec4 operator-(Vec4 v);
}