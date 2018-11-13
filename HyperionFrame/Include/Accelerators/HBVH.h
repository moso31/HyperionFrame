#pragma once
#include "HScene.h"

struct HBVHAABBInfo
{
	int index;
	AABB data;
};

class HBVHTree
{
public:
	bool BuildTreeWithScene(HScene* scene);
	void BuildRecursive();

private:
};