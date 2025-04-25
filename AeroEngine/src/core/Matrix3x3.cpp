#include "AEPCH.h"

namespace AE::Math {
	AE::Math::Matrix3::Matrix3() : Matrix3(0,0,0,0,0,0,0,0,0)
	{

	}
	Matrix3::Matrix3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22)
		: m00(m00), m10(m10), m20(m20), m01(m01), m11(m11), m21(m21), m02(m02), m12(m12), m22(m22)
	{
		
	}
	Matrix3::Matrix3(float numbers[])
	{
		//Be careful with this constructor, will fill with garbage values if the array is smaller than 9
		for (int i = 0; i < 9; ++i) {
			m[i] = numbers[i];
		}
	}

	Matrix3 Matrix3::Transposed() const
	{
		//returns a Column major transposition of the matrix
		return Matrix3(m00, m01, m02,
					   m10, m11, m12,
					   m20, m21, m22);
	}
	float& Matrix3::At(int index)
	{
		return m[std::clamp(index, 0, 8)];
	}

	Matrix3 Matrix3::Minor()
	{
		Matrix3 minor = {
			(m11 * m22 - m21 * m12), (m10 * m22 - m20 * m12), (m10 * m21 - m20 * m11),
			(m01 * m22 - m21 * m02), (m00 * m22 - m20 * m02), (m00 * m21 - m20 * m01),
			(m01 * m12 - m11 * m02), (m00 * m12 - m10 * m02), (m00 * m11 - m10 * m01)
		};

		return minor;
	}

	Matrix3 Matrix3::Cofactor()
	{
		return Matrix3(m00, -m01, m02,
					   -m10, m11, -m12,
					   m20, -m21, m22);
	}

	Matrix3 Matrix3::Adjoint()
	{
		Matrix3 adjoint = Minor();
		adjoint = adjoint.Cofactor();
		return adjoint.Transposed();
	}

	float Matrix3::Determinant()
	{
		return ((m00 * m11 * m22) + (m01 * m12 * m20) + (m02 * m10 * m21)
			- (m00 * m12 * m21) - (m01 * m10 * m22) - (m02 * m11 * m20));
	}

	Matrix3 Matrix3::Inverse()
	{
		Matrix3 adjoint = Adjoint();
		float det = Determinant();
		adjoint /= det;

		return adjoint;
	}

	Matrix3::operator float* () const
	{
		return const_cast<float*>(m);
	}
	Matrix3& Matrix3::operator+=(Matrix3 rhs)
	{
		for (int i = 0; i < 9; ++i) {
			m[i] += rhs.m[i];
		}
		return *this;
	}
	Matrix3& Matrix3::operator-=(Matrix3 rhs)
	{
		for (int i = 0; i < 9; ++i) {
			m[i] -= rhs.m[i];
		}
		return *this;
	}
	Matrix3& Matrix3::operator*=(Matrix3 rhs)
	{
		*this = *this * rhs;
		return *this;
	}
	Matrix3& Matrix3::operator/=(Matrix3 rhs)
	{
		*this *= rhs.Inverse();
		return *this;
	}
	Matrix3& Matrix3::operator/=(float s)
	{
		*this = *this / s;
		return *this;
	}
	Vec3& Matrix3::operator[](int row)
	{
		return axes[std::clamp(row, 0, 2)];
	}
	const Vec3& Matrix3::operator[](int row) const
	{
		return axes[std::clamp(row, 0, 2)];
	}

	Vec3 operator*(Matrix3 lhs, Vec3 rhs)
	{
		return Vec3(Dot(lhs[0], rhs), Dot(lhs[1], rhs), Dot(lhs[2], rhs));
	}

	Matrix3 operator*(Matrix3 lhs, Matrix3 rhs)
	{
		rhs = rhs.Transposed();

		Matrix3 result;
		for (int r = 0; r < 3; ++r) {
			//Dot each row
			for (int c = 0; c < 3; ++c) {

				float cell = Dot(lhs[r], rhs[c]);
				bool flip = cell < 0;

				cell = fabs(cell) < SMALL_NUMBER ? 0.0f : cell; //clamp to zero if near 0
				cell = 1 - fabs(cell) < SMALL_NUMBER ? 1.0f : cell; //clamp to 1 if near 1

				if (flip) cell = -cell;

				result[r][c] = cell;
			}
		}
		return result;
	}
	Matrix3 operator*(Matrix3 m3, float s)
	{
		Matrix3 result;
		for (int i = 0; i < 9; ++i) {
			result.m[i] = m3.m[i] * s;
		}
		return result;
	}
	Matrix3 operator/(Matrix3 m3, float s)
	{
		Matrix3 result;
		for (int i = 0; i < 9; ++i) {
			result.m[i] = m3.m[i] / s;
		}
		return result;
	}
	Matrix3 operator+(Matrix3 lhs, Matrix3 rhs)
	{
		Matrix3 result;
		for (int i = 0; i < 9; ++i) {
			result.m[i] = rhs.m[i] + lhs.m[i];
		}
		return result;
	}
	Matrix3 operator-(Matrix3 lhs, Matrix3 rhs)
	{
		Matrix3 result;
		for (int i = 0; i < 9; ++i) {
			result.m[i] = rhs.m[i] - lhs.m[i];
		}
		return result;
	}
	Matrix3 operator-(Matrix3 m3)
	{
		return m3.Inverse();
	}
}