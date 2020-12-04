#include "../Headers/pch.h"
#include "../Headers/Quaternion.h"
#include "../Headers/Constants.h"

namespace engine 
{
	Qtrn::Qtrn()	
	{
		this->t = 0.f;
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	Qtrn::Qtrn(float theta, float x, float y, float z)
	{
		this->t = theta;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Qtrn::Qtrn(float theta, const Vec4& axis)
	{
		Vec4 axisn = normalize(axis);

		
		float a = theta * (float)DEGREES_TO_RADIANS;
		this->t = cos(a / 2.0f);
		float s = sin(a / 2.0f);
		this->x = axisn.x * s;
		this->y = axisn.y * s;
		this->z = axisn.z * s;

		clean(*this);
		normalize(*this);
	}

	float* Qtrn::data()
	{
		return &t;
	}

	const void toAngleAxis(const Qtrn& q, OUT float& theta, OUT Vec4& axis)
	{
		Qtrn qn = normalize(q);
		theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
		float s = sqrt(1.0f - qn.t * qn.t);
		if (s < DELTA) {
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
			axis.w = 1.0f;
		}
		else {
			float sinv = 1 / s;
			axis.x = qn.x * sinv;
			axis.y = qn.y * sinv;
			axis.z = qn.z * sinv;
			axis.w = 1.0f;
		}
	}

	Qtrn& Qtrn::operator=(const Qtrn& q)
	{
		this->t = q.t;
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
		return *this;
	}


	const void clean(Qtrn& q)
	{
		if (fabs(q.t) < DELTA) q.t = 0.0f;
		if (fabs(q.x) < DELTA) q.x = 0.0f;
		if (fabs(q.y) < DELTA) q.y = 0.0f;
		if (fabs(q.z) < DELTA) q.z = 0.0f;
	}

	const float quadrance(const Qtrn& q)
	{
		return q.t * q.t + q.x * q.x + q.y * q.y + q.z * q.z;
	}

	const float norm(const Qtrn& q)
	{
		return sqrt(quadrance(q));
	}

	const Qtrn normalize(const Qtrn& q)
	{
		float s = 1 / norm(q);
		return q * s;
	}

	const Qtrn conjugate(const Qtrn& q)
	{
		Qtrn qconj = { q.t, -q.x, -q.y, -q.z };
		return qconj;
	}

	const Qtrn inverse(const Qtrn& q)
	{
		return conjugate(q) * (1.0f / quadrance(q));
	}

	const Qtrn operator+(const Qtrn& q0, const Qtrn& q1)
	{
		Qtrn q;
		q.t = q0.t + q1.t;
		q.x = q0.x + q1.x;
		q.y = q0.y + q1.y;
		q.z = q0.z + q1.z;
		return q;
	}

	const Qtrn operator*(const Qtrn& q, const float s)
	{
		Qtrn sq;
		sq.t = s * q.t;
		sq.x = s * q.x;
		sq.y = s * q.y;
		sq.z = s * q.z;
		return sq;
	}

	const Qtrn operator*(const float s, const Qtrn& q)
	{
		Qtrn sq;
		sq.t = s * q.t;
		sq.x = s * q.x;
		sq.y = s * q.y;
		sq.z = s * q.z;
		return sq;
	}

	const Qtrn operator*(const Qtrn& q0, const Qtrn& q1)
	{
		Qtrn q;
		q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
		q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
		q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
		q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
		return q;
	}

	const bool operator==(const Qtrn& q0, const Qtrn& q1)
	{
		return (fabs(q0.t - q1.t) < DELTA && fabs(q0.x - q1.x) < DELTA &&
			fabs(q0.y - q1.y) < DELTA && fabs(q0.z - q1.z) < DELTA);
	}

	const bool operator!=(const Qtrn& q0, const Qtrn& q1)
	{
		return !(q0 == q1);
	}

	const Mat4 GLRotationMatrix(const Qtrn& q)
	{
		Qtrn qn = normalize(q);

		float xx = qn.x * qn.x;
		float xy = qn.x * qn.y;
		float xz = qn.x * qn.z;
		float xt = qn.x * qn.t;
		float yy = qn.y * qn.y;
		float yz = qn.y * qn.z;
		float yt = qn.y * qn.t;
		float zz = qn.z * qn.z;
		float zt = qn.z * qn.t;

		Mat4 matrix;
		matrix.data[0] = 1.0f - 2.0f * (yy + zz);
		matrix.data[1] = 2.0f * (xy + zt);
		matrix.data[2] = 2.0f * (xz - yt);
		matrix.data[3] = 0.0f;

		matrix.data[4] = 2.0f * (xy - zt);
		matrix.data[5] = 1.0f - 2.0f * (xx + zz);
		matrix.data[6] = 2.0f * (yz + xt);
		matrix.data[7] = 0.0f;

		matrix.data[8] = 2.0f * (xz + yt);
		matrix.data[9] = 2.0f * (yz - xt);
		matrix.data[10] = 1.0f - 2.0f * (xx + yy);
		matrix.data[11] = 0.0f;

		matrix.data[12] = 0.0f;
		matrix.data[13] = 0.0f;
		matrix.data[14] = 0.0f;
		matrix.data[15] = 1.0f;

		clean(matrix);

		return matrix;
	}

	const Qtrn lerp(const Qtrn& q0, const Qtrn& q1, const float k)
	{
		float cos_angle = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
		float k0 = 1.0f - k;
		float k1 = (cos_angle > 0) ? k : -k;
		Qtrn qi = (q0 * k0) + (q1 * k1);
		return normalize(qi);
	}

	const Qtrn slerp(const Qtrn& q0, const Qtrn& q1, const float k)
	{
		float angle = acos(q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t);
		float k0 = sin((1 - k) * angle) / sin(angle);
		float k1 = sin(k * angle) / sin(angle);
		Qtrn qi = (q0 * k0) + (q1 * k1);
		return normalize(qi);
	}

	std::ostream& operator<<(std::ostream& os, const Qtrn& q)
	{
		os << "Quaternion = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
		return os;
	}

	const void printAngleAxis(const std::string& s, const Qtrn& q)
	{
		std::cout << s << " = [" << std::endl;

		float thetaf;
		Vec4 axis_f;
		toAngleAxis(q, thetaf, axis_f);
		std::cout << "  angle = " << thetaf << std::endl;
		std::cout << "  axis " << axis_f << std::endl;
		std::cout << "]" << std::endl;
	}
}