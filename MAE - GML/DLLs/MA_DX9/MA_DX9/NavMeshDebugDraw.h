#pragma once

#include "DebugDraw.h"
#include <d3dx9.h>
#include <vector>

struct DVertex
{
	D3DXVECTOR3 position;
	D3DCOLOR color;

	DVertex(float x, float y, float z, D3DCOLOR color) : position(x, y, z), color(color) {}
};

class NavMeshDebugDraw : public duDebugDraw
{
private:
	std::vector<DVertex> Vertices;
	duDebugDrawPrimitives DrawType;

public:
	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
	virtual void end();
};
