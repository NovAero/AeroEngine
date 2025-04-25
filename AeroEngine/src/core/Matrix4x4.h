#pragma once
#include "Vec4.h"

namespace AE::Math {

	class Matrix4 {
	public:

		Matrix4();
		Matrix4(float m00, float m10, float m20, float m30, float m01, float m11, float m21, float m31, float m02, float m12, float m22, float m32, float m03, float m13, float m23, float m33);
		Matrix4(float numbers[]);

	public:

		Vec4 GetRow(int index) const;

		void SetScaled(float x, float y, float z);
		void SetRotateX(float radians);
		void Translate(float x, float y, float z);

		operator float* () const;

		bool operator==(const Matrix4& rhs) const;
		bool operator!=(const Matrix4& rhs) const;
		bool IsEqual(const Matrix4& rhs, const float precision = 0.0001f) const;

		Vec4 operator*(const Vec4& v) const;
		Matrix4 operator*(const Matrix4& other) const;

		Vec4& operator[](int dim);
		const Vec4& operator[](int dim) const;

	public:

		union {
			struct { //ROW,COLUMN
				float m00, m10, m20, m30,//X axis
					m01, m11, m21, m31,//Y axis
					m02, m12, m22, m32,//Z axis
					m03, m13, m23, m33;//Translation
			};

			struct {
				Vec4 xAxis;
				Vec4 yAxis;
				Vec4 zAxis;
				Vec4 translation;
			};

			float m[16];
			float mm[4][4];
			Vec4 axis[4];
		};

	};

}