#include "Vector3.h"
#include <math.h>
#include "Matrix4.h"

Vector3 Vector3::operator+(const Vector3& a) const {
	return Vector3(this->x + a.x, this->y + a.y, this->z + a.z);
}

Vector3 Vector3::operator-(const Vector3& a) const {
	return Vector3(this->x - a.x, this->y - a.y, this->z - a.z);
}

Vector3 Vector3::operator*(const float a) const {
	return Vector3(this->x * a, this->y * a, this->z * a);
}

Vector3 operator*(const float a, const Vector3& b) {
	return Vector3(b.x * a, b.y * a, b.z * a);
}

Vector3 Vector3::operator/(const float a) const {
	return Vector3(this->x / a, this->y / a, this->z / a);
}

float Vector3::Dot(const Vector3& a) const {
	return this->x * a.x + this->y * a.y + this->z * a.z;
}

Vector3 Vector3::Cross(const Vector3& a) const {
	float x = this->y * a.z - this->z * a.y;
	float y = this->z * a.x - this->x * a.z;
	float z = this->x * a.y - this->y * a.x;
	return Vector3(x, y, z);
}

float Vector3::Length() const {
	return sqrtf(LengthSq());
}

float Vector3::LengthSq() const {
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

Vector3& Vector3::Normalize() {
	float length = this->Length();
	if (length != 0) {
		this->x = this->x / length;
		this->y = this->y / length;
		this->z = this->z / length;
	}
	else {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}
	return *this;
}

Vector3 Vector3::GetNormalize() const {
	float length = this->Length();
	float dx = 0;
	float dy = 0;
	float dz = 0;
	if (length != 0) {
		dx = this->x / length;
		dy = this->y / length;
		dz = this->z / length;
	}
	return Vector3(dx, dy, dz);
}

Vector3& Vector3::operator*=(const Matrix4& m)
{
	Vector3 temp = Vector3(*this);
	temp.x = this->x * m.m[0][0] + this->y * m.m[1][0] + this->z * m.m[2][0];
	temp.y = this->x * m.m[0][1] + this->y * m.m[1][1] + this->z * m.m[2][1];
	temp.z = this->x * m.m[0][2] + this->y * m.m[1][2] + this->z * m.m[2][2];
	this->x = temp.x;
	this->y = temp.y;
	this->z = temp.z;
	return *this;
}
