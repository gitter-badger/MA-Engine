#include "Main.h"
#include "Types.h"
#include "Resources.h"
#include <Vec.h>
#include <Mat.h>

DLLEXPORT double MADX9_ShaderCreateASM(const char* VSCode, const char* PSCode)
{
	Shader* shd = new Shader();
	if (!shd->compileasm(VSCode, PSCode))
	{
		delete shd;
		return -1;
	}

	for (uint i = 0; i < mamain->Shader.size(); ++i)
	{
		if (mamain->Shader[i] == 0)
		{
			mamain->Shader[i] = shd;
			return i;
		}
	}

	mamain->Shader.push_back(shd);
	return mamain->Shader.size() - 1;
}

DLLEXPORT double MADX9_ShaderCreateHLSL9(const char* VSCode, const char* PSCode)
{
	Shader* shd = new Shader();

	//Compile the Shader
	if (!shd->compile(VSCode, PSCode))
	{
		delete shd;
		return -1;
	}

	for (uint i = 0; i < mamain->Shader.size(); ++i)
	{
		if (mamain->Shader[i] == 0)
		{
			mamain->Shader[i] = shd;
			return i;
		}
	}

	mamain->Shader.push_back(shd);
	return mamain->Shader.size() - 1;
}

DLLEXPORT double MADX9_ShaderSet(double index)
{
	if ((uint)index > mamain->Shader.size())
		return 0;

	if (mamain->Shader[(uint)index] == 0)
		return 0;

	HRESULT result;

	//Store the current Fixed Vertex Format.
	mamain->d3ddev->GetFVF(&mamain->stFVF);

	//Set the Vertex Shader
	result = mamain->d3ddev->SetVertexShader(mamain->Shader[(uint)index]->VShader);

	if (FAILED(result))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorD3D9, result, "SetVertexShader");
	
	//Set the Pixel Shader
	result = mamain->d3ddev->SetPixelShader(mamain->Shader[(uint)index]->PShader);

	if (FAILED(result))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorD3D9, result, "SetPixelShader");

	return 1;
}

DLLEXPORT double MADX9_ShaderReset()
{
	HRESULT result;

	//Reset the Vertex Shader.
	result = mamain->d3ddev->SetVertexShader(NULL);

	if (FAILED(result))
		return 0;

	//Reset the Pixel Shader.
	result = mamain->d3ddev->SetPixelShader(NULL);

	if (FAILED(result))
		return 0;

	mamain->d3ddev->SetFVF(mamain->stFVF);
	return 1;
}

DLLEXPORT double MADX9_ShaderDestroy(double index)
{
	if ((uint)index > mamain->Shader.size())
		return 0;

	if (mamain->Shader[(uint)index] == 0)
		return 0;

	delete mamain->Shader[(uint)index];
	mamain->Shader[(uint)index] = 0;

	return 1;
}

DLLEXPORT double MADX9_ShaderFindConstant(double index, double shd, const char* c)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->find(c);
}

DLLEXPORT double MADX9_ShaderGetSampler(double index, double shd, double c)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	uint val = (shd ? mamain->Shader[(uint)index]->PConstants : mamain->Shader[(uint)index]->VConstants).getSampler((uint)c);

	return val == ShaderConstants::InvalidSampler ? -1 : val;
}

DLLEXPORT double MADX9_ShaderSetConstantFloat(double index, double shd, double c, double x)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setFloat((uint)c, (float)x);
}

DLLEXPORT double MADX9_ShaderSetConstantVec2(double index, double shd, double c, double x, double y)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setVec2((uint) c, vec2((float) x, (float) y));
}

DLLEXPORT double MADX9_ShaderSetConstantVec3(double index, double shd, double c, double x, double y, double z)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setVec3((uint) c, vec3((float) x, (float) y, (float) z));
}

DLLEXPORT double MADX9_ShaderSetConstantVec4(double index, double shd, double c, double x, double y, double z, double w)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setVec4((uint) c, vec4((float) x, (float) y, (float) z, (float) w));
}

DLLEXPORT double MADX9_ShaderSetConstantMat3(double index, double shd, double c)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setMat3((uint)c, mat3(mamain->matStack.data()));
}

DLLEXPORT double MADX9_ShaderSetConstantMat4(double index, double shd, double c)
{
	if ((uint)index >= mamain->Shader.size())
		return -1;

	if (mamain->Shader[(uint)index] == 0)
		return -1;

	ShaderConstants* t = shd ? &mamain->Shader[(uint)index]->PConstants : &mamain->Shader[(uint)index]->VConstants;

	return t->setMat4((uint)c, mat4(mamain->matStack.data()));
}

DLLEXPORT double MADX9_MatStackFloat8(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8)
{
	mamain->matStack.reserve(8);

	mamain->matStack.push_back((float) v1);
	mamain->matStack.push_back((float) v2);
	mamain->matStack.push_back((float) v3);
	mamain->matStack.push_back((float) v4);
	mamain->matStack.push_back((float) v5);
	mamain->matStack.push_back((float) v6);
	mamain->matStack.push_back((float) v7);
	mamain->matStack.push_back((float) v8);

	return 1;
}

DLLEXPORT double MADX9_MatStackFloat9(double v1, double v2, double v3, double v4, double v5, double v6, double v7, double v8, double v9)
{
	mamain->matStack.reserve(9);

	mamain->matStack.push_back((float) v1);
	mamain->matStack.push_back((float) v2);
	mamain->matStack.push_back((float) v3);
	mamain->matStack.push_back((float) v4);
	mamain->matStack.push_back((float) v5);
	mamain->matStack.push_back((float) v6);
	mamain->matStack.push_back((float) v7);
	mamain->matStack.push_back((float) v8);
	mamain->matStack.push_back((float) v9);

	return 1;
}

DLLEXPORT double MADX9_MatStackClear()
{
	mamain->matStack.clear();

	return 1;
}
