#include "ShaderStructures.h"

class PipelineManager
{
public:
	PipelineManager();
	~PipelineManager();

public:
	static ModelViewProjectionConstantBuffer s_constantBufferData;
};
