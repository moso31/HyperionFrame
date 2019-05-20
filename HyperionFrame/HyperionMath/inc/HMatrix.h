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

	HMatrix4x4 Set(HFloat m11, HFloat m12, HFloat m13, HFloat m14, HFloat m21, HFloat m22, HFloat m23, HFloat m24, HFloat m31, HFloat m32, HFloat m33, HFloat m34, HFloat m41, HFloat m42, HFloat m43, HFloat m44);

	HMatrix4x4 SetTranslation(HFloat x, HFloat y, HFloat z);
	// 绕轴朝向顺时针旋转。
	HMatrix4x4 SetRotationAxis(const HVector3& axis, HFloat angle);
	// 绕法向量朝向顺时针旋转。需要保证旋转轴必须是单位向量。可以节省一次初始化运算。
	HMatrix4x4 SetRotationNormal(const HVector3& normal, HFloat angle);
	// 绕四元数朝向旋转。
	HMatrix4x4 SetRotationQuaternion(const HQuaternion& q);

	// 绕X轴旋转对应角度。
	HMatrix4x4 SetRotationX(HFloat angle);
	// 绕Y轴旋转对应角度。
	HMatrix4x4 SetRotationY(HFloat angle);
	// 绕Z轴旋转对应角度。
	HMatrix4x4 SetRotationZ(HFloat angle);
	// 依次绕XYZ轴朝向顺时针旋转对应的度数。x-pitch, y-yaw, z-roll
	HMatrix4x4 SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll);

	HMatrix4x4 SetScale(HFloat x, HFloat y, HFloat z);
	
	HMatrix4x4 Transpose() const;
	HMatrix4x4 Inverse() const;

	// 下列摄像机相关函数均采用左手坐标系。
	HMatrix4x4 LookAt(const HVector3& eyePos, const HVector3& focusPos, const HVector3& upDir) const;
	HMatrix4x4 LookTo(const HVector3& eyePos, const HVector3& eyeDir, const HVector3& upDir) const;
	HMatrix4x4 Ortho() const;
	HMatrix4x4 Persp() const;

public:
	union
	{
		struct { HFloat _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44; };
		HVector4 v[4];
	};
};
