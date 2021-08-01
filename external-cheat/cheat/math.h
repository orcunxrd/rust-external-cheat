#pragma once
#include <algorithm>
#include <Windows.h>
#include "Vector.h"
#define PI 3.1415927f


static int screenX = GetSystemMetrics(SM_CXSCREEN);
static int screenY = GetSystemMetrics(SM_CYSCREEN);

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};


static Vector3 WorldToScreen2(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenX * .5f;
	float y = screenY * .5f;

	x += 0.5f * _x * screenX + 0.5f;
	y -= 0.5f * _y * screenY + 0.5f;

	return { x,y,w };
}
static Vector3 WorldToScreen(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[1][0] * pos.y + matrix[2][0] * pos.z + matrix[3][0];
	float _y = matrix[0][1] * pos.x + matrix[1][1] * pos.y + matrix[2][1] * pos.z + matrix[3][1];

	float w = matrix[0][3] * pos.x + matrix[1][3] * pos.y + matrix[2][3] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenX * .5f;
	float y = screenY * .5f;

	x += 0.5f * _x * screenX + 0.5f;
	y -= 0.5f * _y * screenY + 0.5f;

	return { x,y,w };
}


__forceinline float Calc3D_Dist(const Vector3& Src, const Vector3& Dst) {
	return sqrtf(pow((Src.x - Dst.x), 2) + pow((Src.y - Dst.y), 2) + pow((Src.z - Dst.z), 2));
}

static float DifferenceOfAngles(Vector2 to, Vector2 from)
{
	Vector2 vdifference;
	vdifference.y = from.y - to.y;
	vdifference.x = from.x - to.x;

	//normalize by making them positive values if they are negative
	if (vdifference.y < 0)
	{
		vdifference.y *= -1;
	}
	if (vdifference.x < 0)
	{
		vdifference.x *= -1;
	}

	//add them together and divide by 2, gives an average of the 2 angles
	float fDifference = (vdifference.y + vdifference.x) / 2;
	return fDifference;
}


static Vector3 Subtract(Vector3 src, Vector3 dst)
{
	Vector3 diff;
	diff.x = src.x - dst.x;
	diff.y = src.y - dst.y;
	diff.z = src.z - dst.z;
	return diff;
}

static float Magnitude(Vector3 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static float Distance(Vector3 src, Vector3 dst)
{
	Vector3 diff = Subtract(src, dst);
	return Magnitude(diff);
}

static float to_radian(float degree)
{
	return degree * 3.141592f / 180.f;
}

static float to_degree(float radian)
{
	return radian * 180.f / 3.141592f;
}

inline float Length(Vector3 v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static bool Normalize(float& Yaw, float& Pitch) {
	//Vector2 CurrentViewAngles = LocalPlayer.BasePlayer->GetVA();
	if (Pitch < -89)
		//Pitch = -89;
		return false;

	else if (Pitch > 89)
		//Pitch = 89;
		return false;

	if (Yaw < -360)
		/*Yaw += 360;*/
		return false;

	else if (Yaw > 360)
		//Yaw -= 360;
		return false;
}
static Vector3 CalcAngle(Vector3 LocalPos, Vector3 EnemyPos)
{
	Vector3 dir;/* = Vector3(LocalPos.x - EnemyPos.x, LocalPos.y - EnemyPos.y, LocalPos.z - EnemyPos.z);*/

	dir.x = LocalPos.x - EnemyPos.x;
	dir.y = LocalPos.y - EnemyPos.y;
	dir.z = LocalPos.z - EnemyPos.z;

	float Pitch = to_degree(asin(dir.y / Length(dir)));
	float Yaw = to_degree(-atan2(dir.x, -dir.z));

	//Normalize(Yaw, Pitch);

	Vector3 Return;
	Return.x = Pitch;
	Return.y = Yaw;

	//return { Pitch, Yaw };
	return Return;
}


Vector4 calculate_quaternion(Vector3 euler)
{
	Vector4 result{};

	auto yaw = (euler.z * 0.01745329251f) * 0.5f;
	auto sy = std::sin(yaw);
	auto cy = std::cos(yaw);

	auto roll = (euler.x * 0.01745329251f) * 0.5f;
	auto sr = std::sin(roll);
	auto cr = std::cos(roll);

	constexpr auto sp = 0.f;
	constexpr auto cp = 1.f;

	result.x = cy * sr * cp - sy * cr * sp;
	result.y = cy * cr * sp + sy * sr * cp;
	result.z = sy * cr * cp - cy * sr * sp;
	result.w = cy * cr * cp + sy * sr * sp;

	return result;
}

static Vector3 CalcAngle4(Vector3 src, Vector3 dst)
{
	Vector3 angle;
	angle.x = -atan2f(dst.x - src.x, dst.y - src.y) / PI * 180.0f + 180.0f;
	angle.y = asinf((dst.z - src.z) / Distance(src, dst)) * 180.0f / PI;
	angle.z = 0.0f;

	return angle;
}
static Vector3 CalcAngle3(Vector3 Source, Vector3 Direction)
{
	Vector3 Angle;
	Vector3 NewAngle;

	Angle.x = Direction.x - Source.x;
	Angle.y = Direction.y - Source.y;
	Angle.z = Direction.z - Source.z;

	float Magnitude = sqrt(Angle.x * Angle.x + Angle.y * Angle.y + Angle.z * Angle.z);

	NewAngle.x = (float)atan2(Angle.y, Angle.x) * 180.0 / PI;
	NewAngle.y = -atan2(Angle.z, Magnitude) * 180.0 / PI;
	NewAngle.z = 0;

	return NewAngle;
}
static void CalcAngle2(float* src, float* dst, float* angles)
{
	float deltaX = (src[0]) - (dst[0]);
	float deltaY = (src[1]) - (dst[1]);

	if ((dst[0]) > (src[0]) && (dst[1]) <= (src[1]))
	{
		angles[0] = atanf(deltaX / deltaY) * -180.0f / PI;
	}

	else if ((dst[0]) >= (src[0]) && (dst[1]) > (src[1]))
	{
		angles[0] = atanf(deltaX / deltaY) * -180.0f / PI + 180.0f;
	}

	else if ((dst[0]) < (src[0]) && (dst[1]) >= (src[1]))
	{
		angles[0] = atanf(deltaX / deltaY) * -180.0f / PI - 180.0f;
	}

	else if ((dst[0]) <= (src[0]) && (dst[1]) < (src[1]))
	{
		angles[0] = atanf(deltaX / deltaY) * -180.0f / PI + 360.0f;
	}

	float deltaZ = (dst[2]) - (src[2]);
	float dist = sqrt(
		pow(double(dst[0] - src[0]), 2.0) +
		pow(double(dst[1] - src[1]), 2.0) +
		pow(double(dst[2] - src[2]), 2.0));

	angles[1] = asinf(deltaZ / dist) * 180.0f / PI;
}

static float pythag(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}