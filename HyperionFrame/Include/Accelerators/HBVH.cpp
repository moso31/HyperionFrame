#include "HBVH.h"

bool HBVHTree::BuildTreeWithScene(HScene * scene)
{
	AABB aabb = scene->GetAABB();
	int dim = aabb.GetMaximumExtent();

	vector<HBVHAABBInfo> boundInfo;
	for (vector<Shape*>::iterator it = scene->shapes.begin(), int count = 0; it != scene->shapes.end(); it++, count++)
	{
		HBVHAABBInfo info;
		info.data = (*it)->GetAABB();
		info.index = count;
		boundInfo.push_back(info);
	}

	return false;
}
