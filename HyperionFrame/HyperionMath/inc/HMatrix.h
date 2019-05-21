#pragma once
#include "HVector.h"

class HMatrix4x4
{
public:
	HMatrix4x4() {}
	HMatrix4x4(HFloat _11, HFloat _12, HFloat _13, HFloat _14, HFloat _21, HFloat _22, HFloat _23, HFloat _24, HFloat _31, HFloat _32, HFloat _33, HFloat _34, HFloat _41, HFloat _42, HFloat _43, HFloat _44);
	HMatrix4x4(const HMatrix4x4& m);
	~HMatrix4x4() {}
	
	HMatrix4x4 operator+ (const HMatrix4x4& m) const;
	HMatrix4x4 operator- (const HMatrix4x4& m) const;
	HMatrix4x4 operator* (const HMatrix4x4& m) const;
	HVector4 operator[] (const HInt& index) { return v[index]; }

	void SetIdentity();
	void SetZero();
	void SetNaN();

	HMatrix4x4 Set(const HFloat m11, const HFloat m12, const HFloat m13, const HFloat m14, const HFloat m21, const HFloat m22, const HFloat m23, const HFloat m24, const HFloat m31, const HFloat m32, const HFloat m33, const HFloat m34, const HFloat m41, const HFloat m42, const HFloat m43, const HFloat m44);

	HMatrix4x4 SetTranslation(HFloat x, HFloat y, HFloat z);
	// 绕轴朝向屏幕外为准，顺时针旋转angle度。
	HMatrix4x4 SetRotationAxis(const HVector3& axis, HFloat angle);
	// 绕法向量朝向屏幕外为准，顺时针旋转angle度。
	// 需要保证旋转轴必须是单位向量。可以节省一次初始化运算。
	HMatrix4x4 SetRotationNormal(const HVector3& normal, HFloat angle);
	// 绕四元数朝向旋转。
	HMatrix4x4 SetRotationQuaternion(const HQuaternion& q);

	// 以X轴朝向屏幕外为准，顺时针旋转angle度。
	HMatrix4x4 SetRotationX(HFloat angle);
	// 以Y轴朝向屏幕外为准，顺时针旋转angle度。
	HMatrix4x4 SetRotationY(HFloat angle);
	// 以Z轴朝向屏幕外为准，顺时针旋转angle度。
	HMatrix4x4 SetRotationZ(HFloat angle);
	// 依次绕XYZ轴朝向屏幕外为准，顺时针旋转对应的度数。x-pitch, y-yaw, z-roll
	HMatrix4x4 SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll);

	HMatrix4x4 SetScale(HFloat x, HFloat y, HFloat z);
	
	HMatrix4x4 Transpose() const;
	HMatrix4x4 Inverse() const;

	HMatrix4x4 SetLookAtLH(const HVector3& eyePos, const HVector3& focusPos, const HVector3& upDir);
	HMatrix4x4 SetLookToLH(const HVector3& eyePos, const HVector3& eyeDir, const HVector3& upDir);
	HMatrix4x4 SetOrthoLH(const HFloat width, const HFloat height, const HFloat zNear, const HFloat zFar);
	HMatrix4x4 SetPerspLH(const HFloat width, const HFloat height, const HFloat zNear, const HFloat zFar);

public:
	union
	{
		struct { HFloat _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };
		HVector4 v[4];
	};
};
