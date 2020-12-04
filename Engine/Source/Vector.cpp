#include "../Headers/pch.h"
#include "../Headers/Vector.h"
#include "../Headers/Constants.h"


namespace engine
{
	/////////////////////////////////////////////////////////////////////// Vec2
	Vec2::Vec2()
	{
		this->x = 0.f;
		this->y = 0.f;
	}

	Vec2::Vec2(const float& k)
	{
		this->x = k;
		this->y = k;
	}

	Vec2::Vec2(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}

	const float* Vec2::data()
	{
		return &x;
	}

	const float magnitude(const Vec2& vector)
	{
		float x = pow(vector.x, 2);
		float y = pow(vector.y, 2);
		return sqrt(x + y);
	}

	const Vec2 normalize(const Vec2& vector)
	{
		float mag = magnitude(vector);
		return Vec2(vector.x / mag, vector.y / mag);
	}

	const float Vec2::dotProduct(const Vec2& other)
	{
		return (this->x * other.x + this->y * other.y);
	}

	const Vec2 operator+(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x + right.x, left.y + right.y);
	}

	const Vec2 operator-(const Vec2& left, const Vec2& right)
	{
		return Vec2(left.x - right.x, left.y - right.y);
	}

	const Vec2 operator*(const Vec2& vector, const float scalar)
	{
		return Vec2(vector.x * scalar, vector.y * scalar);
	}

	const Vec2 operator*(const float scalar, const Vec2& vector)
	{
		return Vec2(vector.x * scalar, vector.y * scalar);
	}

	const Vec2 operator/(const Vec2& Vector, const float scalar)
	{
		return Vec2(Vector.x / scalar, Vector.y / scalar);
	}

	const bool operator==(const Vec2& left, const Vec2& right)
	{

		return fabs(left.x - right.x) <= DELTA && fabs(left.y - right.y) <= DELTA;
	}

	const bool operator!=(const Vec2& left, const Vec2& right)
	{
		return !(left == right);
	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	Vec2& Vec2::operator+=(const Vec2& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}

	Vec2& Vec2::operator*=(const float& scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const Vec2& Vector)
	{
		stream << "Vector 2D : (" << Vector.x << ", " << Vector.y << ")";
		return stream;
	}

	std::istream& operator>>(std::istream& stream, Vec2& Vector)
	{
		std::cout << "Type x coordenate: ";
		stream >> Vector.x;
		std::cout << "Type y coordenate: ";
		stream >> Vector.y;
		return stream;
	}

	/////////////////////////////////////////////////////////////////////// Vec3

	Vec3::Vec3()
	{
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	Vec3::Vec3(const float& k)
	{
		this->x = k;
		this->y = k;
		this->z = k;
	}

	Vec3::Vec3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	const float* Vec3::data()
	{
		return &x;
	}

	const float magnitude(const Vec3& vector)
	{
		float x = pow(vector.x, 2);
		float y = pow(vector.y, 2);
		float z = pow(vector.z, 2);
		return sqrt(x + y + z);
	}

	const Vec3 normalize(const Vec3& vector)
	{
		float mag = magnitude(vector);
		return Vec3(vector.x / mag, vector.y / mag, vector.z / mag);
	}

	const float Vec3::dotProduct(const Vec3& other)
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z);
	}

	const Vec3 Vec3::crossProduct(const Vec3& other)
	{
		float x = (this->y * other.z) - (this->z * other.y);
		float y = (this->z * other.x) - (this->x * other.z);
		float z = (this->x * other.y) - (this->y * other.x);
		return Vec3(x, y, z);
	}

	Vec3& Vec3::operator+=(const Vec3& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}

	Vec3& Vec3::operator*=(const float& scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	Vec3& Vec3::operator=(const Vec3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	const Vec3 operator+(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	const Vec3 operator-(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	const Vec3 operator*(const Vec3& vector, const float scalar)
	{
		return Vec3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
	}

	const Vec3 operator*(const float scalar, const Vec3& vector)
	{
		return Vec3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
	}

	const Vec3 operator/(const Vec3& left, const float right)
	{
		return Vec3(left.x / right, left.y / right, left.z / right);
	}

	const bool operator==(const Vec3& left, const Vec3& right)
	{
		return fabs(left.x - right.x) <= DELTA && fabs(left.y - right.y) <= DELTA && fabs(left.z - right.z) <= DELTA;
	}

	const bool operator!=(const Vec3& left, const Vec3& right)
	{
		return !(left == right);
	}

	std::ostream& operator<<(std::ostream& stream, const Vec3& Vector)
	{
		stream << "Vector 3D: (" << Vector.x << ", " << Vector.y << ", " << Vector.z << ")";
		return stream;
	}

	std::istream& operator>>(std::istream& stream, Vec3& Vector)
	{
		std::cout << "Type x coordenate: ";
		stream >> Vector.x;
		std::cout << "Type y coordenate: ";
		stream >> Vector.y;
		std::cout << "Type z coordenate: ";
		stream >> Vector.z;
		return stream;
	}

	/////////////////////////////////////////////////////////////////////// Vec4

	Vec4::Vec4()
	{
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
		this->w = 1.f;
	}

	Vec4::Vec4(const float& k)
	{
		this->x = k;
		this->y = k;
		this->z = k;
		this->w = k;
	}

	Vec4::Vec4(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 1.f;
	}

	Vec4::Vec4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	const float* Vec4::data()
	{
		return &x;
	}

	const float magnitude(const Vec4& vector)
	{
		float x = pow(vector.x, 2);
		float y = pow(vector.y, 2);
		float z = pow(vector.z, 2);
		//float w = pow(this->w, 2);
		return sqrt(x + y + z);
	}

	const Vec4 normalize(const Vec4& vector)
	{
		float mag = magnitude(vector);
		return Vec4(vector.x / mag, vector.y / mag, vector.z / mag, vector.w);
	}

	const float Vec4::dotProduct(const Vec4& other)
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z);
	}

	const Vec4 Vec4::crossProduct(const Vec4& other)
	{
		return Vec4(y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x);
	}

	const Vec4 operator+(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	const Vec4 operator-(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	const Vec4 operator*(const Vec4& vector, const float scalar)
	{
		return Vec4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
	}

	const Vec4 operator*(const float scalar, const Vec4& vector)
	{
		return Vec4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
	}

	const Vec4 operator/(const Vec4& vector, const float scalar)
	{
		return Vec4(vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar);
	}

	const bool operator==(const Vec4& left, const Vec4& right)
	{
		return fabs(left.x - right.x) <= DELTA && fabs(left.y - right.y) <= DELTA
			&& fabs(left.z - right.z) <= DELTA && fabs(left.w - right.w) <= DELTA;
	}

	const bool operator!=(const Vec4& left, const Vec4& right)
	{
		return !(left == right);
	}

	Vec4& Vec4::operator+=(const Vec4& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		return *this;
	}

	Vec4& Vec4::operator-=(const Vec4& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		return *this;
	}

	Vec4& Vec4::operator*=(const float& scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	Vec4& Vec4::operator=(const Vec4& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}

	std::ostream& operator<<(std::ostream& stream, const Vec4& Vector)
	{
		stream << "Vector 4D: (" << Vector.x << ", " << Vector.y << ", " << Vector.z << ", " << Vector.w << ")";
		return stream;
	}

	std::istream& operator>>(std::istream& stream, Vec4& Vector)
	{
		std::cout << "Type x coordenate: ";
		stream >> Vector.x;
		std::cout << "Type y coordenate: ";
		stream >> Vector.y;
		std::cout << "Type z coordenate: ";
		stream >> Vector.z;
		std::cout << "Type w coordenate: ";
		stream >> Vector.w;
		return stream;
	}

	/////////////////////////////////////////////////////////////////////// convertions

	const Vec3 to3D(const Vec2& Vector)
	{
		return Vec3(Vector.x, Vector.y, 0.f);
	}
	const Vec4 to4D(const Vec2& Vector)
	{
		return Vec4(Vector.x, Vector.y, 0.f);
	}
	const Vec2 to2D(const Vec3& Vector)
	{
		return Vec2(Vector.x / Vector.z, Vector.y / Vector.z);
	}
	const Vec4 to4D(const Vec3& Vector)
	{
		return Vec4(Vector.x, Vector.y, Vector.z);
	}
	const Vec2 to2D(const Vec4& Vector)
	{
		return Vec2(Vector.x / Vector.z, Vector.y / Vector.z);
	}
	const Vec3 to3D(const Vec4& Vector)
	{
		return Vec3(Vector.x/Vector.w, Vector.y/Vector.w, Vector.z/ Vector.w);
	}
}