#pragma once

namespace engine
{
	/////////////////////////////////////////////////////////////////////// 
	struct Vec2;
	struct Vec3;
	struct Vec4;
	/////////////////////////////////////////////////////////////////////// Vec2
	struct Vec2
	{
		float x, y;

		Vec2();
		Vec2(const float& k);
		Vec2(const float& x, const float& y);

		const float* data();

		friend const float magnitude(const Vec2& vector);
		friend const Vec2 normalize(const Vec2& vector);

		const float dotProduct(const Vec2& other);

		friend const Vec2 operator + (const Vec2& left, const Vec2& right);
		friend const Vec2 operator - (const Vec2& left, const Vec2& right);
		friend const Vec2 operator * (const Vec2& vector, const float scalar);
		friend const Vec2 operator * (const float scalar, const Vec2& Vector);
		friend const Vec2 operator / (const Vec2& left, const float right);
		friend const bool operator == (const Vec2& left, const Vec2& right);
		friend const bool operator != (const Vec2& left, const Vec2& right);

		Vec2& operator += (const Vec2& other);
		Vec2& operator -= (const Vec2& other);
		Vec2& operator *= (const float& scalar);
		Vec2& operator = (const Vec2& other);

		friend std::ostream& operator << (std::ostream& stream, const Vec2& vector);
		friend std::istream& operator >> (std::istream& stream, Vec2& vector);
	};

	const Vec2 AXIS2D_X = { 1.0f, 0.0f };
	const Vec2 AXIS2D_Y = { 1.0f, 1.0f };

	/////////////////////////////////////////////////////////////////////// Vec3
	struct Vec3
	{
		float x, y, z;

		Vec3();
		Vec3(const float& k);
		Vec3(const float& x, const float& y, const float& z);

		const float* data();

		friend const float magnitude(const Vec3& vector);
		friend const Vec3 normalize(const Vec3& vector);

		const float dotProduct(const Vec3& other);
		const Vec3 crossProduct(const Vec3& other);

		friend const Vec3 operator + (const Vec3& left, const Vec3& right);
		friend const Vec3 operator - (const Vec3& left, const Vec3& right);
		friend const Vec3 operator * (const Vec3& Vector, const float scalar);
		friend const Vec3 operator * (const float scalar, const Vec3& Vector);
		friend const Vec3 operator / (const Vec3& left, const float right);
		friend const bool operator == (const Vec3& left, const Vec3& right);
		friend const bool operator != (const Vec3& left, const Vec3& right);

		//const Vec3 rotate(Vec3& vector, const float angle);

		Vec3& operator += (const Vec3& other);
		Vec3& operator -= (const Vec3& other);
		Vec3& operator *= (const float& scalar);
		Vec3& operator = (const Vec3& other);

		friend std::ostream& operator << (std::ostream& stream, const Vec3& Vector);
		friend std::istream& operator >> (std::istream& stream, Vec3& Vector);
	};

	const Vec3 AXIS3D_X = { 1.0f, 0.0f , 0.0f };
	const Vec3 AXIS3D_Y = { 0.0f, 1.0f , 0.0f };
	const Vec3 AXIS3D_Z = { 0.0f, 0.0f , 1.0f };

	/////////////////////////////////////////////////////////////////////// Vec4
	struct Vec4
	{
		float x, y, z, w;

		Vec4();
		Vec4(const float& k);
		Vec4(const float& x, const float& y, const float& z);
		Vec4(const float& x, const float& y, const float& z, const float& w);

		const float* data();

		friend const float magnitude(const Vec4& vector);
		friend const Vec4 normalize(const Vec4& vector);

		const float dotProduct(const Vec4& other);
		const Vec4 crossProduct(const Vec4& other);

		friend const Vec4 operator + (const Vec4& left, const Vec4& right);
		friend const Vec4 operator - (const Vec4& left, const Vec4& right);
		friend const Vec4 operator * (const Vec4& vector, const float scalar);
		friend const Vec4 operator * (const float scalar, const Vec4& vector);
		friend const Vec4 operator / (const Vec4& left, const float right);
		friend const bool operator == (const Vec4& left, const Vec4& right);
		friend const bool operator != (const Vec4& left, const Vec4& right);


		Vec4& operator += (const Vec4& other);
		Vec4& operator -= (const Vec4& other);
		Vec4& operator *= (const float& scalar);
		Vec4& operator = (const Vec4& other);

		friend std::ostream& operator << (std::ostream& stream, const Vec4& Vector);
		friend std::istream& operator >> (std::istream& stream, Vec4& Vector);
	};

	const Vec4 AXIS4D_X = { 1.0f, 0.0f , 0.0f , 1.0f };
	const Vec4 AXIS4D_Y = { 0.0f, 1.0f , 0.0f , 1.0f };
	const Vec4 AXIS4D_Z = { 0.0f, 0.0f , 1.0f , 1.0f };

	/////////////////////////////////////////////////////////////////////// convertions

	const Vec2 to2D(const Vec3& Vector);
	const Vec2 to2D(const Vec4& Vector);
	const Vec3 to3D(const Vec2& Vector);
	const Vec3 to3D(const Vec4& Vector);
	const Vec4 to4D(const Vec2& Vector);
	const Vec4 to4D(const Vec3& Vector);
	
	
	

}