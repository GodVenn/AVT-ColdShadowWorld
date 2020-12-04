#include "../Headers/pch.h"
#include "../Headers/Matrix.h"
#include "../Headers/Constants.h"

namespace engine
{
	/////////////////////////////////////////////////////////////////////// Mat2
	Mat2::Mat2()
	{
		for (int i = 0; i < 4; i++) {
			this->data[i] = 0.f;
		}
		this->data[0] = this->data[1 + 1 * 2] = 1.0f;
		
	}

	Mat2::Mat2(const float k)
	{
		for (int i = 0; i < 4; i++) {
			this->data[i] = 0;
		}
		this->data[0] = this->data[1 + 1 * 2] = k;
	}

	Mat2::Mat2(const float m0, const float m2, 
			   const float m1, const float m3)
	{
		// 2x2 matrix - column major
		//    0    2
		//    1    3
		this->data[0] = m0;
		this->data[1] = m1;
		this->data[2] = m2;
		this->data[3] = m3;
	}

	Mat2::Mat2(const float matrixData[])
	{
		this->data[0] = matrixData[0];
		for (int i = 0; i < 4; i++) 
		{
			this->data[i] = matrixData[i];
		}
	}

	Mat2& Mat2::operator=(const Mat2& m)
	{
		for (int i = 0; i < 4; i++)
		{
			this->data[i] = m.data[i];
		}
		return *this;
	}

	const void clean(Mat2& matrix)
	{
		for (int i = 0; i < 4; i++) {
			if (fabs(matrix.data[i]) < DELTA) matrix.data[i] = 0.0f;
		}
	}

	const Mat2 transpose(const Mat2& matrix)
	{
		Mat2 transpose;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				transpose.data[i + j * 2] = matrix.data[j + i * 2];
			}
		}
		return transpose;
	}

	const float determinant(const Mat2& matrix)
	{
		return matrix.data[0] * matrix.data[3] - matrix.data[1] * matrix.data[2];
	}

	const Mat2 inverse(const Mat2& matrix)
	{
		if (determinant(matrix) == 0) 
		{
			std::cout << "Imposible to calculate inverse, determinant equal to 0" << std::endl;
			return matrix;
		}
		
		return  1 / determinant(matrix) * Mat2(matrix.data[3], -matrix.data[2], -matrix.data[1], matrix.data[0]);
	}

	const Mat2 operator+(const Mat2& m0, const Mat2& m1)
	{
		Mat2 result;
		for (int i = 0; i < 4; i++) {
			result.data[i] = m0.data[i] + m1.data[i];
		}
		return result;
	}

	const Mat2 operator-(const Mat2& m0, const Mat2& m1)
	{
		Mat2 result;
		for (int i = 0; i < 4; i++) {
			result.data[i] = m0.data[i] - m1.data[i];
		}
		return result;
	}

	const Mat2 operator*(const float k, const Mat2& m)
	{
		Mat2 result = Mat2();
		for (int i = 0; i < 4; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Mat2 operator*(const Mat2& m, const float k)
	{
		Mat2 result;
		for (int i = 0; i < 4; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Vec2 operator*(const Mat2& m, const Vec2& v)
	{
		Vec2 result;
		result.x = v.x * m.data[0] + v.y * m.data[0 + 1 * 2];
		result.y = v.x * m.data[1 + 0 * 2] + v.y * m.data[1 + 1 * 2];
		return result;
	}

	const Mat2 operator*(const Mat2& m0, const Mat2& m1)
	{
		Mat2 result;
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				float sum = 0.0f;
				for (int e = 0; e < 2; e++) {
					sum += m0.data[x + e * 2] * m1.data[e + y * 2];
				}
				result.data[x + y * 2] = sum;
			}
		}
		return result;
		
	}

	const bool operator==(const Mat2& m0, const Mat2& m1)
	{
		for (int i = 0; i < 2; i++) {
			
			if (fabs(m0.data[i] - m1.data[i]) > DELTA)
			{
				return false;
			}
		}
		return true;
	}

	const bool operator!=(const Mat2& m0, const Mat2& m1)
	{
		return !(m0 == m1);
	}

	std::ostream& operator<<(std::ostream& os, const Mat2& m)
	{
		os << "2D Matrix: " << std::endl;
		os << "| " << m.data[0 + 0 * 2] << "    " << m.data[0 + 1 * 2] << " |" << std::endl;
		os << "| " << m.data[1 + 0 * 2] << "    " << m.data[1 + 1 * 2] << " |" << std::endl;
		return os;
	}

	void columnMajorFormat(const Mat2& m, GLfloat* data)
	{
		for (int i = 0; i < 4; i++)
		{
			data[i] = m.data[i];
		}
	}

	/////////////////////////////////////////////////////////////////////// Mat3
	Mat3::Mat3()
	{
		for (int i = 0; i < 9; i++) {
			this->data[i] = 0.0f;
		}
		this->data[0] = this->data[1 + 1 * 3] = this->data[2 + 2 * 3] = 1.0f;
	}

	Mat3::Mat3(const float k)
	{
		for (int i = 0; i < 9; i++) {
			this->data[i] = 0;
		}
		this->data[0] = this->data[1 + 1 * 3] = this->data[2 + 2 * 3] = k;
	}

	Mat3::Mat3(const float m0, const float m3, const float m6, 
				const float m1, const float m4, const float m7, 
				const float m2, const float m5, const float m8)
	{
		// 3x3 matrix - column major
		//    0    3    6   
		//    1    4    7   
		//    2    5    8
		this->data[0] = m0;
		this->data[1] = m1;
		this->data[2] = m2;

		this->data[3] = m3;
		this->data[4] = m4;
		this->data[5] = m5;

		this->data[6] = m6;
		this->data[7] = m7;
		this->data[8] = m8;
	}

	Mat3::Mat3(const float matrixData[])
	{
		this->data[0] = matrixData[0];
		for (int i = 0; i < 9; i++) {
			this->data[i] = matrixData[i];
		}
	}

	Mat3& Mat3::operator=(const Mat3& m)
	{
		for (int i = 0; i < 9; i++)
		{
			this->data[i] = m.data[i];
		}
		return *this;
	}

	

	const void clean(Mat3& matrix)
	{
		for (int i = 0; i < 9; i++) {
			if (fabs(matrix.data[i]) < DELTA) matrix.data[i] = 0.0f;
		}
	}

	const Mat3 transpose(const Mat3& matrix)
	{
		Mat3 transpose;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				transpose.data[i + j * 3] = matrix.data[j + i * 3];
			}
		}
		return transpose;
	}

	const float determinant(const Mat3& matrix)
	{
		
		return matrix.data[0] * (matrix.data[4] * matrix.data[8] - (matrix.data[7] * matrix.data[5]))
			- matrix.data[3] * (matrix.data[1] * matrix.data[8] - (matrix.data[2] * matrix.data[7]))
			+ matrix.data[6] * (matrix.data[1] * matrix.data[5] - (matrix.data[2] * matrix.data[4]));
	}

	const Mat3 inverse(const Mat3& matrix)
	{
		float d = determinant(matrix);
		if (d == 0)
		{
			std::cout << "Imposible to calculate inverse, determinant equal to 0" << std::endl;
			return matrix;
		}

		Mat3 mat = transpose(matrix);

		Mat3 minorMatrices = mat;
		float minorMatrixDeterminant = 0.f;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++) 
			{
				float minorMatrix[4] = {0,0,0,0};
				int index = 0;

				for (int k = 0; k < 3; k++)
				{
					for (int w = 0;  w < 3;  w++)
					{
						if (k != i  && w != j)
						{
							minorMatrix[index] = mat.data[k + w * 3];
							index++;
						}
					}

				}

				minorMatrixDeterminant = determinant(Mat2(minorMatrix[0], minorMatrix[1], minorMatrix[2], minorMatrix[3]));

				minorMatrices.data[i + j * 3] = minorMatrixDeterminant;
			}
		}

		return 1/d * adjoint(minorMatrices);
	}

	const Mat3 adjoint(const Mat3& matrix)
	{
		return Mat3(matrix.data[0], -matrix.data[0 + 1 * 3], matrix.data[0 + 2 * 3], 
					-matrix.data[1 + 0 * 3], matrix.data[1 + 1 * 3], -matrix.data[1 + 2 * 3], 
			         matrix.data[2 + 0 * 3], -matrix.data[2 + 1 * 3], matrix.data[2 + 2 * 3]);
	}

	const Mat3 operator+(const Mat3& m0, const Mat3& m1)
	{
		Mat3 result;
		for (int i = 0; i < 9; i++) {
			result.data[i] = m0.data[i] + m1.data[i];
		}
		return result;
	}

	const Mat3 operator-(const Mat3& m0, const Mat3& m1)
	{
		Mat3 result;
		for (int i = 0; i < 9; i++) {
			result.data[i] = m0.data[i] - m1.data[i];
		}
		return result;
	}

	const Mat3 operator*(const float k, const Mat3& m)
	{
		Mat3 result;
		for (int i = 0; i < 9; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Mat3 operator*(const Mat3& m, const float k)
	{
		Mat3 result;
		for (int i = 0; i < 9; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Vec3 operator*(const Mat3& m, const Vec3& v)
	{
		Vec3 result;
		result.x = v.x * m.data[0 + 0 * 3] + v.y * m.data[0 + 1 * 3] + v.z * m.data[0 + 2 * 3];
		result.y = v.x * m.data[1 + 0 * 3] + v.y * m.data[1 + 1 * 3] + v.z * m.data[1 + 2 * 3];
		result.z = v.x * m.data[2 + 0 * 3] + v.y * m.data[2 + 1 * 3] + v.z * m.data[2 + 2 * 3];
		return result;
	}

	const Mat3 operator*(const Mat3& m0, const Mat3& m1)
	{
		Mat3 result;
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				float sum = 0.0f;
				for (int e = 0; e < 3; e++) {
					sum += m0.data[x + e * 3] * m1.data[e + y * 3];
				}
				result.data[x + y * 3] = sum;
			}
		}
		return result;
	}

	const bool operator==(const Mat3& m0, const Mat3& m1)
	{
		for (int i = 0; i < 9; i++) {

			if (fabs(m0.data[i] - m1.data[i]) > DELTA)
			{
				return false;
			}
		}
		return true;
	}

	const bool operator!=(const Mat3& m0, const Mat3& m1)
	{
		return !(m0 == m1);
	}

	std::ostream& operator<<(std::ostream& os, const Mat3& m)
	{
		os << "3D Matrix: " << std::endl;
		os << "| " << m.data[0 + 0 * 3] << "    " << m.data[0 + 1 * 3] << "    " << m.data[0 + 2 * 3] << " |" << std::endl;
		os << "| " << m.data[1 + 0 * 3] << "    " << m.data[1 + 1 * 3] << "    " << m.data[1 + 2 * 3] << " |" << std::endl;
		os << "| " << m.data[2 + 0 * 3] << "    " << m.data[2 + 1 * 3] << "    " << m.data[2 + 2 * 3] << " |" << std::endl;
		return os;
	}

	void columnMajorFormat(const Mat3& m, GLfloat* data)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] = m.data[i];
		}
		
	}

	void rowMajorFormat(const Mat3& m, GLfloat* data)
	{
		for(int i = 0; i < 9; i++)
		{
			data[i] = m.data[i];
		}
		
	}

	/////////////////////////////////////////////////////////////////////// Mat4
	Mat4::Mat4()
	{
		for (int i = 0; i < 16; i++) {
			this->data[i] = 0.0f;
		}
		this->data[0] = this->data[1 + 1 * 4] = this->data[2 + 2 * 4] = this->data[3 + 3 * 4] = 1.0f;
	}

	Mat4::Mat4(const float k)
	{
		for (int i = 0; i < 16; i++)
		{
			this->data[i] = 0.f;
		}
		this->data[0] = this->data[1 + 1 * 4] = this->data[2 + 2 * 4] = this->data[3 + 3 * 4] = k;
	}

	Mat4::Mat4(const float m0, const float m4, const float m8, const float m12,
			   const float m1, const float m5, const float m9, const float m13, 
		       const float m2, const float m6, const float m10, const float m14, 
		       const float m3, const float m7, const float m11, const float m15)
	{
		// 4x4 matrix - column major
		//    0    4    8    12
		//    1    5    9    13
		//    2    6    10   14
		//    3    7    11   15
		this->data[0] = m0;
		this->data[1] = m1;
		this->data[2] = m2;
		this->data[3] = m3;

		this->data[4] = m4;
		this->data[5] = m5;
		this->data[6] = m6;
		this->data[7] = m7;

		this->data[8] = m8;
		this->data[9] = m9;
		this->data[10] = m10;
		this->data[11] = m11;

		this->data[12] = m12;
		this->data[13] = m13;
		this->data[14] = m14;
		this->data[15] = m15;

	}

	Mat4::Mat4(const float matrixData[])
	{
		this->data[0] = matrixData[0];
		for (int i = 0; i < 16; i++) {
			this->data[i] = matrixData[i];
		}
	}

	Mat4& Mat4::operator=(const Mat4& m)
	{
		for (int i = 0; i < 16; i++)
		{
			this->data[i] = m.data[i];
		}
		return *this;
	}

	const void clean(Mat4& matrix)
	{
		for (int i = 0; i < 16; i++) {
			if (fabs(matrix.data[i]) < DELTA) matrix.data[i] = 0.0f;
		}
	}

	const Mat4 transpose(const Mat4& matrix)
	{
		Mat4 transpose;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j ++) 
			{
				transpose.data[i + j * 4] = matrix.data[j + i * 4];
			}
		}
		return transpose;
	}

	const Mat4 operator+(const Mat4& m0, const Mat4& m1)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++) {
			result.data[i] = m0.data[i] + m1.data[i];
		}
		return result;
	}

	const Mat4 operator-(const Mat4& m0, const Mat4& m1)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++) {
			result.data[i] = m0.data[i] - m1.data[i];
		}
		return result;
	}

	const Mat4 operator*(const float k, const Mat4& m)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Mat4 operator*(const Mat4& m, const float k)
	{
		Mat4 result;
		for (int i = 0; i < 16; i++) {
			result.data[i] = m.data[i] * k;
		}
		return result;
	}

	const Vec4 operator*(const Mat4& m, const Vec4& v)
	{
		Vec4 result;
		result.x = v.x * m.data[0] + v.y * m.data[0 + 1 * 4] + v.z * m.data[0 + 2 * 4] + v.w * m.data[0 + 3 * 4];
		result.y = v.x * m.data[1 + 0 * 4] + v.y * m.data[1 + 1 * 4] + v.z * m.data[1 + 2 * 4] + v.w * m.data[1 + 3 * 4];
		result.z = v.x * m.data[2 + 0 * 4] + v.y * m.data[2 + 1 * 4] + v.z * m.data[2 + 2 * 4] + v.w * m.data[2 + 3 * 4];
		result.w = v.x * m.data[3 + 0 * 4] + v.y * m.data[3 + 1 * 4] + v.z * m.data[3 + 2 * 4] + v.w * m.data[3 + 3 * 4];
		return result;
	}

	const Mat4 operator*(const Mat4& m0, const Mat4& m1)
	{
		Mat4 result;
		for (int y = 0; y < 4; y++) {
			for (int x = 0; x < 4; x++) {
				float sum = 0.0f;
				for (int e = 0; e < 4; e++) {
					sum += m0.data[x + e * 4] * m1.data[e + y * 4];
				}
				result.data[x + y * 4] = sum;
			}
		}
		return result;
	}

	const bool operator==(const Mat4& m0, const Mat4& m1)
	{
		for (int i = 0; i < 16; i++) {

			if (fabs(m0.data[i] - m1.data[i]) > DELTA)
			{
				return false;
			}
		}
		return true;
	}

	const bool operator!=(const Mat4& m0, const Mat4& m1)
	{
		return !(m0 == m1);
	}

	std::ostream& operator<<(std::ostream& os, const Mat4& m)
	{
		os << "4D Matrix: " << std::endl;
		os << "| " << m.data[0 + 0 * 4] << "    " << m.data[0 + 1 * 4] << "    " << m.data[0 + 2 * 4] << "    " << m.data[0 + 3 * 4] << " |" << std::endl;
		os << "| " << m.data[1 + 0 * 4] << "    " << m.data[1 + 1 * 4] << "    " << m.data[1 + 2 * 4] << "    " << m.data[1 + 3 * 4] << " |" << std::endl;
		os << "| " << m.data[2 + 0 * 4] << "    " << m.data[2 + 1 * 4] << "    " << m.data[2 + 2 * 4] << "    " << m.data[2 + 3 * 4] << " |" << std::endl;
		os << "| " << m.data[3 + 0 * 4] << "    " << m.data[3 + 1 * 4] << "    " << m.data[3 + 2 * 4] << "    " << m.data[3 + 3 * 4] << " |" << std::endl;
		return os;
	}

	void columnMajorFormat(const Mat4& m, GLfloat* data)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] = m.data[i];
		}
	}

	/////////////////////////////////////////////////////////////////////// MatFactory

	const Mat2 MatFactory::createZeroMat2()
	{
		return Mat2(0.f);
	}

	const Mat2 MatFactory::createIdentityMat2()
	{
		return Mat2();
	}

	const Mat2 MatFactory::createScaleMat2(const Vec2& v)
	{	
		Mat2 scaleMatrix = Mat2();
		scaleMatrix.data[0] = v.x;
		scaleMatrix.data[1 + 1 * 2] = v.y;
		return scaleMatrix;
	}

	const Mat2 MatFactory::createRotationMat2(const float angle)
	{
		float ang = angle * DEGREES_TO_RADIANS;
		return Mat2(cos(ang), -sin(ang), 
				sin(ang), cos(ang));
	}

	const Mat3 MatFactory::createZeroMat3()
	{
		return Mat3(0.f);
	}

	const Mat3 MatFactory::createIdentityMat3()
	{
		return Mat3();
	}

	const Mat3 MatFactory::createScaleMat3(const Vec3& v)
	{
		Mat3 scaleMatrix = Mat3();
		scaleMatrix.data[0] = v.x;
		scaleMatrix.data[1 + 1 * 3] = v.y;
		scaleMatrix.data[2 + 2 * 3] = v.z;

		return scaleMatrix;
	}

	const Mat3 MatFactory::createRotationMat3(const float angle,const Vec3& v)
	{
		float ang = angle * DEGREES_TO_RADIANS;
		Vec3 normalized = normalize(v);
		Mat3 indentity = Mat3();
		Mat3 dualMatrix = createDualMat3(normalized);

		return indentity + sin(ang) * dualMatrix + (1 - cos(ang)) * dualMatrix * dualMatrix;
	}

	const Mat3 MatFactory::createDualMat3(const Vec3& v)
	{
		return Mat3(0.f, -v.z, v.y,
					v.z, 0.f, -v.x,
					-v.y, v.x, 0.f);
	}

	const Mat3 MatFactory::toMat3(const Mat4& m)
	{
		Mat3 result;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				result.data[x + y * 3] = m.data[x + y * 4];
			}
		}
		return result;
	}

	const Mat4 MatFactory::createZeroMat4()
	{
		return Mat4(0.f);
	}

	const Mat4 MatFactory::createIdentityMat4()
	{
		return Mat4();
	}

	const Mat4 MatFactory::createScaleMat4(const Vec3& v)
	{
		Mat4 scaleMatrix = Mat4();
		scaleMatrix.data[0] = v.x;
		scaleMatrix.data[1 + 1 * 4] = v.y;
		scaleMatrix.data[2 + 2 * 4] = v.z;
		
		return scaleMatrix;
	}

	const Mat4 MatFactory::createRotationMat4(const float angle, const Vec3& v)
	{
		Mat3 rot3D = createRotationMat3(angle, v);
		Mat4 result = Mat4();

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				result.data[i + j * 4] = rot3D.data[i + j * 3];
			}
		}
		return result;
	}

	const Mat4 MatFactory::createTranslationMat4(const Vec3& v)
	{
		Mat4 translationMatrix = Mat4();
		translationMatrix.data[0 + 3 * 4] = v.x;
		translationMatrix.data[1 + 3 * 4] = v.y;
		translationMatrix.data[2 + 3 * 4] = v.z;
		
		return translationMatrix;
	}
	const Mat4 MatFactory::toMat4(const Mat3& m)
	{
		Mat4 result = Mat4();

		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				result.data[x + y * 4] = m.data[x + y * 3];
			}
		}
		return result;
	}

	const Mat4 MatFactory::createViewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up)
	{
		Vec3 e = eye;
		Vec3 v = normalize(center - e);
		Vec3 s = normalize(v.crossProduct(up));
		Vec3 u = s.crossProduct(v);
		return Mat4(s.x, s.y, s.z, -(e.dotProduct(s)),
					u.x, u.y, u.z,   -(e.dotProduct(u)),
				    -v.x, -v.y, -v.z, e.dotProduct(v),
					0.f, 0.f, 0.f, 1.f);
	}

	const Mat4 MatFactory::createOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far)
	{
		return Mat4(2.f / (right - left), 0.f, 0.f, (left + right) / (left - right),
			0.f, 2.f / (top - bottom), 0.f, (bottom + top) / (bottom - top),
			0.f, 0.f, 2.f / (near - far), (near + far) / (near - far),
			0.f, 0.f, 0.f, 1.f);
	}
	const Mat4 MatFactory::createPerspectiveProjectionMatrix(const float fovy, const float ratio, const float near, const float far)
	{
		float ang = fovy * DEGREES_TO_RADIANS;
		float d = 1 / tan(ang/2);

		return Mat4(d/ratio, 0.f, 0.f, 0.f,
					0.f, d, 0.f, 0.f,
					0.f, 0.f , (near + far)/(near - far), (2*near*far)/(near - far),
					0.f, 0.f, -1.f, 0.f);
	}
}