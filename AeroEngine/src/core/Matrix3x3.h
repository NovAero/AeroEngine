#pragma once
#include "Vec3.h"

namespace AE::Math {
	class Matrix3 {
	public:

		//==IMPORTANT NOTE==
		//Matrix3 and all other matrices are ROW major in AeroEngine math classes

		Matrix3();
		Matrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22);
		Matrix3(float numbers[]);

	public:

		union {
			struct { // float mROW_COLUMN
				float m00, m10, m20, //X axis
					m01, m11, m21, //Y axis
					m02, m12, m22; //Z axis
			};
			struct {

				Vec3 xAxis;
				Vec3 yAxis;
				Vec3 zAxis;

			};
			float m[9];
			float mm[3][3];
			Vec3 axes[3];
		};

	public:

		Matrix3 Transposed() const;
		float& At(int index);

		Matrix3 Minor();
		Matrix3 Cofactor();
		Matrix3 Adjoint();
		float Determinant();
		Matrix3 Inverse();

		operator float* () const;

		Matrix3& operator+=(Matrix3 rhs);
		Matrix3& operator-=(Matrix3 rhs);
		Matrix3& operator*=(Matrix3 rhs);
		// This operator multiplies m by the inverse of rhs. no division in matrices, just a use of an operator
		Matrix3& operator/=(Matrix3 rhs);
		Matrix3& operator/=(float s);
		Vec3& operator[](int row);
		const Vec3& operator[](int row) const;

	};

	Matrix3 operator*(Matrix3 lhs, Matrix3 rhs);
	Matrix3 operator*(Matrix3 m3, float s);
	Matrix3 operator/(Matrix3 m3, float s);
	Matrix3 operator+(Matrix3 lhs, Matrix3 rhs);
	Matrix3 operator-(Matrix3 lhs, Matrix3 rhs);
	Matrix3 operator-(Matrix3 m3); //Returns inverse of matrix 
	Vec3 operator*(Matrix3 lhs, Vec3 rhs);

}