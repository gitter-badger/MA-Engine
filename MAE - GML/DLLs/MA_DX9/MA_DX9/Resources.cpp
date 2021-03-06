#include "Main.h"
#include "Resources.h"

const uint MaxRenderTargets = 16;

std::stack<LPDIRECT3DSURFACE9> DepthBuffers;
std::stack<LPDIRECT3DSURFACE9> RenderTargets[MaxRenderTargets];
std::stack<D3DVIEWPORT9>       Viewports[MaxRenderTargets];

ShaderConstants::~ShaderConstants() {
	if (constants != 0)
		constants->Release();
}

uint ShaderConstants::find(std::string c) {
	D3DXHANDLE h = constants->GetConstantByName(0, c.c_str());

	if (h == 0)
		return 0;

	for (uint i = 0; i < handles.size(); ++i)
		if (handles[i] == h)
			return i;

	handles.push_back(h);
	return handles.size() - 1;
}

uint ShaderConstants::getSampler(uint c) {
	if (c >= handles.size())
		return InvalidSampler;

	return constants->GetSamplerIndex(handles[c]);
}

bool ShaderConstants::setFloat(uint c, float f) {
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloat(mamain->d3ddev, handles[c], f));
}

bool ShaderConstants::setVec2(uint c, const vec2& v) {
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloatArray(mamain->d3ddev, handles[c], v.data, v.components));
}

bool ShaderConstants::setVec3(uint c, const vec3& v)
{
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloatArray(mamain->d3ddev, handles[c], v.data, v.components));
}

bool ShaderConstants::setVec4(uint c, const vec4& v)
{
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloatArray(mamain->d3ddev, handles[c], v.data, v.components));
}

bool ShaderConstants::setMat3(uint c, const mat3& v)
{
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloatArray(mamain->d3ddev, handles[c], v.data, v.size * v.size));
}

bool ShaderConstants::setMat4(uint c, const mat4& v)
{
	if (c >= handles.size())
		return 0;

	return SUCCEEDED(constants->SetFloatArray(mamain->d3ddev, handles[c], v.data, v.size * v.size));
}

Shader::~Shader() {
	if (VShader != 0)
		VShader->Release();

	if (PShader != 0)
		PShader->Release();
}

ErrorCode Shader::compileasm(std::string vert, std::string pixel) {
	LPD3DXBUFFER code;
	LPD3DXBUFFER err;

	HRESULT result;

	/**
	 * Assemble Vertex Shader
	 */

	result = D3DXAssembleShader(vert.c_str(), vert.length(), NULL, NULL, 0, &code, &err);
		
	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCompileVertexShader, result, (char*) err->GetBufferPointer());
		err->Release();

		return ErrorCompileVertexShader;
	}

	result = mamain->d3ddev->CreateVertexShader((DWORD*) code->GetBufferPointer(), &VShader);
	code->Release();

	if (FAILED(result))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreateVertexShader, result);

	/**
	* Assemble Pixel Shader
	*/

	result = D3DXAssembleShader(pixel.c_str(), pixel.length(), NULL, NULL, 0, &code, &err);

	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCompilePixelShader, result, (char*) err->GetBufferPointer());
		err->Release();

		VShader->Release();
		VShader = 0;

		return ErrorCompilePixelShader;
	}

	result = mamain->d3ddev->CreatePixelShader((DWORD*)code->GetBufferPointer(), &PShader);
	code->Release();

	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreatePixelShader, result);

		VShader->Release();
		VShader = 0;

		return ErrorCreatePixelShader;
	}

	return ErrorOk;
}

ErrorCode Shader::compile(std::string vert, std::string pixel)
{
	LPD3DXBUFFER code;
	LPD3DXBUFFER err;

	HRESULT result;
	
	result = D3DXCompileShader(vert.c_str(), vert.length(), NULL, NULL, "main", D3DXGetVertexShaderProfile(mamain->d3ddev), 0, &code, &err, &VConstants.constants);
	
	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCompileVertexShader, result, (char*) err->GetBufferPointer());
		err->Release();

		return ErrorCompileVertexShader;
	}

	result = mamain->d3ddev->CreateVertexShader((DWORD*) code->GetBufferPointer(), &VShader);

	code->Release();

	if (FAILED(result))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreateVertexShader, result);

	result = D3DXCompileShader(pixel.c_str(), pixel.length(), NULL, NULL, "main", D3DXGetPixelShaderProfile(mamain->d3ddev), 0, &code, &err, &PConstants.constants);

	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCompilePixelShader, result, (char*) err->GetBufferPointer());
		err->Release();
		
		VShader->Release();
		VShader = 0;

		return ErrorCompilePixelShader;
	}

	result = mamain->d3ddev->CreatePixelShader((DWORD*) code->GetBufferPointer(), &PShader);

	code->Release();

	if (FAILED(result))
	{
		ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreatePixelShader, result);
		
		VShader->Release();
		VShader = 0;
		
		return ErrorCreatePixelShader;
	}

	return ErrorOk;
}

Texture::~Texture()
{
	if (tex != 0)
		tex->Release();
}

ErrorCode Texture::create(uint width, uint height, uint levels, uint usage, D3DFORMAT format, D3DPOOL pool)
{
	if (tex != 0)
	{
		tex->Release();
		tex = 0;
	}

	flags = 0;

	HRESULT res = mamain->d3ddev->CreateTexture(width, height, levels, usage, format, pool, &tex, 0);

	if (FAILED(res))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreateTexture, res);

	if (usage & D3DUSAGE_AUTOGENMIPMAP)
		flags |= FlagMipMaps;

	return ErrorOk;
}

ErrorCode Texture::generateMipMaps()
{
	if ((flags & FlagMipMaps) == 0)
		return ErrorHandle(mamain->err, ErrorInv);

	tex->GenerateMipSubLevels();

	return ErrorOk;
}

ErrorCode Texture::getSurface(uint level, Surface& surf)
{
	if (level >= tex->GetLevelCount())
		return ErrorHandle(mamain->err, ErrorInv);

	LPDIRECT3DSURFACE9 s;

	HRESULT res = tex->GetSurfaceLevel(level, &s);

	if (FAILED(res))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorD3D9, res, "GetSurfaceLevel");

	ErrorCode ret = surf.createFromPointer(s);

	s->Release();

	return ret;
}

uint Texture::getSurfaceCount()
{
	return tex->GetLevelCount();
}

ErrorCode Texture::loadFromFile(std::string file, MipMaps mipmaps)
{
	if (tex != 0)
	{
		tex->Release();
		tex = 0;
	}

	flags = 0;

	HRESULT res = D3DXCreateTextureFromFileEx(mamain->d3ddev, file.c_str(), 0, 0, mipmaps == MipMapsGenerate ? 0 : (mipmaps == MipMapsFromFile ? D3DX_FROM_FILE : 1), 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex);

	if (FAILED(res))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreateTexFromFile, res, file);

	return ErrorOk;
}

ErrorCode Texture::loadFromFileInMemory(const void* data, uint length, MipMaps mipmaps)
{
	if (tex != 0)
	{
		tex->Release();
		tex = 0;
	}

	flags = 0;

	HRESULT res = D3DXCreateTextureFromFileInMemoryEx(mamain->d3ddev, data, length, 0, 0, mipmaps == MipMapsGenerate ? 0 : (mipmaps == MipMapsFromFile ? D3DX_FROM_FILE : 1), 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &tex);

	if (FAILED(res))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorCreateTexture, res);

	return ErrorOk;
}

ErrorCode Texture::setMipMapFilter(D3DTEXTUREFILTERTYPE filter)
{
	if ((flags & FlagMipMaps) == 0)
		return ErrorHandle(mamain->err, ErrorInv);

	HRESULT res = tex->SetAutoGenFilterType(filter);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetAutoGenFilterType");

	return ErrorOk;
}

ErrorCode Texture::update(Texture& src)
{
	HRESULT res = mamain->d3ddev->UpdateTexture(src.tex, tex);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "UpdateTexture");

	return ErrorOk;
}

Surface::~Surface()
{
	if (surf != 0)
		surf->Release();
}

ErrorCode Surface::createDepthStencil(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE ms, uint msquality, bool discard)
{
	if (surf != 0)
	{
		surf->Release();
		surf = 0;
	}

	HRESULT res = mamain->d3ddev->CreateDepthStencilSurface(width, height, format, ms, msquality, discard, &surf, 0);

	if (FAILED(res))
		return ErrorHandleCritical(mamain->err, mamain->errCrit, ErrorD3D9, res, "CreateDepthStencilSurface");

	surfUsage  = D3DUSAGE_DEPTHSTENCIL;
	surfPool   = D3DPOOL_DEFAULT;
	surfWidth  = width;
	surfHeight = height;

	return ErrorOk;
}

ErrorCode Surface::createFromPointer(LPDIRECT3DSURFACE9 surf)
{
	if (this->surf != 0)
	{
		this->surf->Release();
		this->surf = 0;
	}

	if (surf == 0)
		return ErrorHandle(mamain->err, ErrorInv);

	D3DSURFACE_DESC desc;

	HRESULT res = surf->GetDesc(&desc);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "GetDesc");

	surfUsage  = desc.Usage;
	surfPool   = desc.Pool;
	surfWidth  = desc.Width;
	surfHeight = desc.Height;

	this->surf = surf;
	surf->AddRef();

	return ErrorOk;
}

ErrorCode Surface::createRenderTarget(uint width, uint height, D3DFORMAT format, D3DMULTISAMPLE_TYPE ms, uint msquality, bool lockable)
{
	if (surf != 0)
	{
		surf->Release();
		surf = 0;
	}

	HRESULT res = mamain->d3ddev->CreateRenderTarget(width, height, format, ms, msquality, lockable, &surf, 0);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "CreateRenderTarget");

	surfUsage  = D3DUSAGE_RENDERTARGET;
	surfPool   = D3DPOOL_DEFAULT;
	surfWidth  = width;
	surfHeight = height;

	return ErrorOk;
}

ErrorCode Surface::setRenderTarget(uint level)
{
	if ((surfUsage & D3DUSAGE_RENDERTARGET) == 0 || level > MaxRenderTargets)
		return ErrorHandle(mamain->err, ErrorInv);

	LPDIRECT3DSURFACE9 surf;

	HRESULT res = mamain->d3ddev->GetRenderTarget(level, &surf);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "GetRenderTarget");

	D3DVIEWPORT9 vp;

	mamain->d3ddev->GetViewport(&vp);

	Viewports[level].push(vp);

	RenderTargets[level].push(surf);

	vp.X      = 0;
	vp.Y      = 0;
	vp.Width  = surfWidth;
	vp.Height = surfHeight;
	vp.MinZ   = 0;
	vp.MaxZ   = 1;

	res = mamain->d3ddev->SetRenderTarget(level, this->surf);

	mamain->d3ddev->SetViewport(&vp);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetRenderTarget");

	return ErrorOk;
}

ErrorCode Surface::resetRenderTarget(uint level)
{
	if (level > MaxRenderTargets || RenderTargets[level].empty())
		return ErrorHandle(mamain->err, ErrorInv);

	LPDIRECT3DSURFACE9 surf = RenderTargets[level].top();

	RenderTargets[level].pop();

	surf->Release();

	HRESULT res = mamain->d3ddev->SetRenderTarget(level, surf);

	mamain->d3ddev->SetViewport(&Viewports[level].top());
	Viewports[level].pop();

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetRenderTarget");

	return ErrorOk;
}

ErrorCode Surface::setDepthBuffer()
{
	if ((surfUsage & D3DUSAGE_DEPTHSTENCIL) == 0)
		return ErrorHandle(mamain->err, ErrorInv);

	LPDIRECT3DSURFACE9 surf;

	HRESULT res = mamain->d3ddev->GetDepthStencilSurface(&surf);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "GetDepthStencilSurface");

	DepthBuffers.push(surf);

	res = mamain->d3ddev->SetDepthStencilSurface(this->surf);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetDepthStencilSurface");

	return ErrorOk;
}

ErrorCode Surface::resetDepthBuffer()
{
	if (DepthBuffers.empty())
		return ErrorHandle(mamain->err, ErrorInv);

	LPDIRECT3DSURFACE9 surf = DepthBuffers.top();

	DepthBuffers.pop();

	surf->Release();

	HRESULT res = mamain->d3ddev->SetDepthStencilSurface(surf);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetDepthStencilSurface");

	return ErrorOk;
}

ErrorCode Surface::update(Surface& surf)
{
	if (surfPool == D3DPOOL_DEFAULT && surf.surfPool == D3DPOOL_DEFAULT)
		return ErrorHandle(mamain->err, ErrorInv);

	HRESULT res;

	if (surfPool == D3DPOOL_DEFAULT)
		res = mamain->d3ddev->UpdateSurface(surf.surf, 0, this->surf, 0);
	else
		res = mamain->d3ddev->GetRenderTargetData(surf.surf, this->surf);

	if (FAILED(res))
		return ErrorHandle(mamain->err, ErrorD3D9, res, "SetDepthStencilSurface");

	return ErrorOk;
}
