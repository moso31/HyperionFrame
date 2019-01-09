#pragma once
#include "Shape.h"

// 特征表的单个特征纪录。
// 如果当前物体具有新特征，则存储到特征表中，如果新物体的特征在特征表中已经存在，则和对应的物体共用同一buffer。
// 此操作用于优化并减少buffer的数量。
struct ShapeCharacter
{
	bool IsSame(const ShapeCharacter &other) const;

	// 物体的类型。
	eShapeType type;
	// 物体的特征参数。可能为空。
	vector<int> params;	
	// 物体的buffer数据。
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

	// 特征表，存储场景中每种物体的特征。
	vector<ShapeCharacter> m_characterTable;

	// shape映射表记录场景shape和特征表的映射关系。
	vector<int> m_shapeMapping;
};
