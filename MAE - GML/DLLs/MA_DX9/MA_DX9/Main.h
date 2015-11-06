#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <D3Dcompiler.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fstream>

#include "Hooks.h"
#include "Error.h"
#include "Resources.h"

#include "Object.h"
#include "ObjectTerrain.h"
#include "ObjectModel.h"

class Exception {
	public:
		Exception(char* error)
		{
			exception = error;
		}
		char* exception;
};

class MADLLMain
{
public:
	MADLLMain();

	void init(LPDIRECT3DDEVICE9 d3ddev);
	void free();
	const char* returnStr(std::string str);
	bool isInitialized();

	Error err;

	LPDIRECT3D9       d3d;
	LPDIRECT3DDEVICE9 d3ddev;

	std::vector<Shader*>   Shader;
	std::vector<D3DLIGHT9> Light;
	std::vector<D3DMATERIAL9> Material;
	std::vector<MD2Model*> MD2Models;
	std::vector<D3DHooks*> Hooks;
	std::vector<Texture*>  Textures;

	DWORD stFVF;
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclarationMD2 = 0;
	LPDIRECT3DVERTEXDECLARATION9 VertexDeclarationMSM = 0;

	/**
	 * This stack can be used to transfer matrices to the DLL using MADX9_MatStackFloat and MADX9_MatStackClear
	 */

	std::vector<float> matStack;

private:
	std::string retStr;

	enum {
		FlagInitialized = 0x01
	};

	uint flags;
};

extern MADLLMain mamain;