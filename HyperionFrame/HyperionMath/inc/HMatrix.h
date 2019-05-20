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
	// ���ᳯ��˳ʱ����ת��
	HMatrix4x4 SetRotationAxis(const HVector3& axis, HFloat angle);
	// �Ʒ���������˳ʱ����ת����Ҫ��֤��ת������ǵ�λ���������Խ�ʡһ�γ�ʼ�����㡣
	HMatrix4x4 SetRotationNormal(const HVector3& normal, HFloat angle);
	// ����Ԫ��������ת��
	HMatrix4x4 SetRotationQuaternion(const HQuaternion& q);

	// ��X����ת��Ӧ�Ƕȡ�
	HMatrix4x4 SetRotationX(HFloat angle);
	// ��Y����ת��Ӧ�Ƕȡ�
	HMatrix4x4 SetRotationY(HFloat angle);
	// ��Z����ת��Ӧ�Ƕȡ�
	HMatrix4x4 SetRotationZ(HFloat angle);
	// ������XYZ�ᳯ��˳ʱ����ת��Ӧ�Ķ�����x-pitch, y-yaw, z-roll
	HMatrix4x4 SetRotationXYZ(HFloat pitch, HFloat yaw, HFloat roll);

	HMatrix4x4 SetScale(HFloat x, HFloat y, HFloat z);
	
	HMatrix4x4 Transpose() const;
	HMatrix4x4 Inverse() const;

	// �����������غ�����������������ϵ��
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
