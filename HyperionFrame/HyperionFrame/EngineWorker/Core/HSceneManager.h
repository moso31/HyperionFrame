#pragma once
#include "Shape.h"

// ������ĵ���������¼��
// �����ǰ�����������������洢���������У��������������������������Ѿ����ڣ���Ͷ�Ӧ�����干��ͬһbuffer��
// �˲��������Ż�������buffer��������
struct ShapeCharacter
{
	bool IsSame(const ShapeCharacter &other) const;

	// ��������͡�
	eShapeType type;
	// �������������������Ϊ�ա�
	vector<int> params;	
	// �����buffer���ݡ�
	ShapeBuffer bufferData;
};

class HSceneManager
{
public:
	HSceneManager();
	HSceneManager(std::shared_ptr<DXResource> dxResources, ComPtr<ID3D12GraphicsCommandList> pCommandList);
	~HSceneManager();

	shared_ptr<Box> CreateBox(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
	shared_ptr<Sphere> CreateSphere(float radius, int segmentHorizontal, int segmentVertical);
	shared_ptr<HMesh> CreateMesh(string filepath);

	int GetShapeCharacterCount();
	int GetShapeBufferIndex(size_t shapeIndex); 

private:
	std::shared_ptr<DXResource> m_dxResources;
	ComPtr<ID3D12GraphicsCommandList> m_pCommandList;

	// �������洢������ÿ�������������
	vector<ShapeCharacter> m_characterTable;

	// shapeӳ����¼����shape���������ӳ���ϵ��
	vector<int> m_shapeMapping;
};
