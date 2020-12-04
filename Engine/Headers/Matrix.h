#pragma once

#include "Vector.h"
#include <GL/glew.h>

namespace engine
{
	///////////////////////////////////////////////////////////////////////
	struct Mat2;
	struct Mat3;
	struct Mat4;
	struct MatFactory;
	/////////////////////////////////////////////////////////////////////// Mat2
	struct Mat2
	{
		float data [4];

		Mat2();
		Mat2(const float k);
		Mat2(const float m0, const float m2, 
			 const float m1, const float m3);
		Mat2(const float matrixData[]);

		friend const void clean(Mat2& matrix);
		friend const Mat2 transpose(const Mat2& matrix);
		friend const float determinant(const Mat2& matrix);
		friend const Mat2 inverse(const Mat2& matrix);

		Mat2& operator = (const Mat2& m);

		friend const Mat2 operator + (const Mat2& m0, const Mat2& m1);
		friend const Mat2 operator - (const Mat2& m0, const Mat2& m1);
		friend const Mat2 operator * (const float k, const Mat2& m);
		friend const Mat2 operator * (const Mat2& m, const float k);
		friend const Vec2 operator * (const Mat2& m, const Vec2& v);
		friend const Mat2 operator * (const Mat2& m0, const Mat2& m1);
		friend const bool operator == (const Mat2& m0, const Mat2& m1);
		friend const bool operator != (const Mat2& m0, const Mat2& m1);
		friend std::ostream& operator << (std::ostream& os, const Mat2& m);

		friend void columnMajorFormat(const Mat2& m, GLfloat* data);
	};

	/////////////////////////////////////////////////////////////////////// Mat3
	struct Mat3
	{
		float data [9];

		Mat3();
		Mat3(const float k);
		Mat3(const float m0, const float m3, const float m6,
			const float m1, const float m4, const float m7,
			const float m2, const float m5, const float m8);
		Mat3(const float matrixData []);

		friend const void clean(Mat3& matrix);
		friend const Mat3 transpose(const Mat3& matrix);
		friend const float determinant(const Mat3& matrix);
		friend const Mat3 adjoint(const Mat3& matrix);
		friend const Mat3 inverse(const Mat3& matrix);	

		Mat3& operator = (const Mat3& m);

		friend const Mat3 operator + (const Mat3& m0, const Mat3& m1);
		friend const Mat3 operator - (const Mat3& m0, const Mat3& m1);
		friend const Mat3 operator * (const float k, const Mat3& m);
		friend const Mat3 operator * (const Mat3& m, const float k);
		friend const Vec3 operator * (const Mat3& m, const Vec3& v);
		friend const Mat3 operator * (const Mat3& m0, const Mat3& m1);
		friend const bool operator == (const Mat3& m0, const Mat3& m1);
		friend const bool operator != (const Mat3& m0, const Mat3& m1);
		friend std::ostream& operator << (std::ostream& os, const Mat3& m);

		friend void columnMajorFormat(const Mat3& m, GLfloat* data);

	};

	/////////////////////////////////////////////////////////////////////// Mat4
	struct Mat4
	{
		float data [16];

		Mat4();
		Mat4(const float k);
		Mat4(const float m0, const float m4, const float m8, const float m12,
			 const float m1, const float m5, const float m9, const float m13,
			 const float m2, const float m6, const float m10, const float m14,
			 const float m3, const float m7, const float m11, const float m15);
		Mat4(const float matrixData[]);

		friend const void clean(Mat4& matrix);
		friend const Mat4 transpose(const Mat4& matrix);

		Mat4& operator = (const Mat4& m);

		friend const Mat4 operator + (const Mat4& m0, const Mat4& m1);
		friend const Mat4 operator - (const Mat4& m0, const Mat4& m1);
		friend const Mat4 operator * (const float k, const Mat4& m);
		friend const Mat4 operator * (const Mat4& m, const float k);
		friend const Vec4 operator * (const Mat4& m, const Vec4& v);
		friend const Mat4 operator * (const Mat4& m0, const Mat4& m1);
		friend const bool operator == (const Mat4& m0, const Mat4& m1);
		friend const bool operator != (const Mat4& m0, const Mat4& m1);
		friend std::ostream& operator << (std::ostream& os, const Mat4& m);		

		friend void columnMajorFormat(const Mat4& m, GLfloat* data);
	};

	/////////////////////////////////////////////////////////////////////// MatFactory
	struct MatFactory
	{
		static const Mat2 createZeroMat2();
		static const Mat2 createIdentityMat2();
		static const Mat2 createScaleMat2(const Vec2& v);
		static const Mat2 createRotationMat2(const float angle);

		static const Mat3 createZeroMat3();
		static const Mat3 createIdentityMat3();
		static const Mat3 createScaleMat3(const Vec3& v);
		static const Mat3 createRotationMat3(const float angle, const Vec3& v);
		static const Mat3 createDualMat3(const Vec3& v);
		static const Mat3 toMat3(const Mat4& m);

		static const Mat4 createZeroMat4();
		static const Mat4 createIdentityMat4();
		static const Mat4 createScaleMat4(const Vec3& v);
		static const Mat4 createRotationMat4(const float angle, const Vec3& v);
		static const Mat4 createTranslationMat4(const Vec3& v);
		static const Mat4 createViewMatrix(const Vec3& eye, const Vec3& center, const Vec3& up);
		static const Mat4 createOrthographicProjectionMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far);
		static const Mat4 createPerspectiveProjectionMatrix(const float fovy, const float ratio, const float near, const float far);
		static const Mat4 toMat4(const Mat3& m);
	}; 
}