#pragma once
#include "HMathType.h"
#include "HVector.h"

class HMatrix4x4
{
public:
	HMatrix4x4() {}
	HMatrix4x4(HFloat _00, HFloat _01, HFloat _02, HFloat _03, HFloat _10, HFloat _11, HFloat _12, HFloat _13, HFloat _20, HFloat _21, HFloat _22, HFloat _23, HFloat _30, HFloat _31, HFloat _32, HFloat _33);
	HMatrix4x4(const HMatrix4x4& m);
	~HMatrix4x4() {}
	
	HMatrix4x4 operator+ (const HMatrix4x4& m) const;
	HMatrix4x4 operator- (const HMatrix4x4& m) const;
	HMatrix4x4 operator* (const HMatrix4x4& m) const;
	HVector4 operator[] (const HInt& index) { return v[index]; }

	HMatrix4x4 SetIdentity();
	HMatrix4x4 SetZero();
	HMatrix4x4 SetNaN();

	HMatrix4x4 SetTranslation(HFloat x, HFloat y, HFloat z);
	// ����˳ʱ����ת��
	HMatrix4x4 SetRotationAxis(const HVector3& axis, HFloat angle);
	// �Ʒ�����˳ʱ����ת����Ҫ��֤��ת������ǵ�λ���������Խ�ʡһ�γ�ʼ�����㡣
	HMatrix4x4 SetRotationNormal(const HVector3& normal, HFloat angle);
	// ����Ԫ����ת��
	HMatrix4x4 SetRotationQuaternion(const HQuaternion& q);
	// ������XYZ��˳ʱ����ת��Ӧ�Ķ�����x-pitch, y-yaw, z-roll
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
		struct { HFloat _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33; };
		HVector4 v[4];
	};
};
