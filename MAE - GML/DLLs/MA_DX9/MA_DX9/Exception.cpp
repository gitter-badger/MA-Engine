#include "Exception.h"

#include <map>
#include <d3d9.h>

std::string MAEDX9Exception::format(uint code, const std::string& msg)
{
	const std::map<uint, std::string> strings = {
		{D3DOK_NOAUTOGEN,                    " (D3DOK_NOAUTOGEN)"},
		{D3DERR_CONFLICTINGRENDERSTATE,      " (D3DERR_CONFLICTINGRENDERSTATE)"},
		{D3DERR_CONFLICTINGTEXTUREFILTER,    " (D3DERR_CONFLICTINGTEXTUREFILTER)"},
		{D3DERR_CONFLICTINGTEXTUREPALETTE,   " (D3DERR_CONFLICTINGTEXTUREPALETTE)"},
		{D3DERR_DEVICEHUNG,                  " (D3DERR_DEVICEHUNG)"},
		{D3DERR_DEVICELOST,                  " (D3DERR_DEVICELOST)"},
		{D3DERR_DEVICENOTRESET,              " (D3DERR_DEVICENOTRESET)"},
		{D3DERR_DEVICEREMOVED,               " (D3DERR_DEVICEREMOVED)"},
		{D3DERR_DRIVERINTERNALERROR,         " (D3DERR_DRIVERINTERNALERROR)"},
		{D3DERR_DRIVERINVALIDCALL,           " (D3DERR_DRIVERINVALIDCALL)"},
		{D3DERR_INVALIDCALL,                 " (D3DERR_INVALIDCALL)"},
		{D3DERR_INVALIDDEVICE,               " (D3DERR_INVALIDDEVICE)"},
		{D3DERR_MOREDATA,                    " (D3DERR_MOREDATA)"},
		{D3DERR_NOTAVAILABLE,                " (D3DERR_NOTAVAILABLE)"},
		{D3DERR_NOTFOUND,                    " (D3DERR_NOTFOUND)"},
		{D3D_OK,                             " (D3D_OK)"},
		{D3DERR_OUTOFVIDEOMEMORY,            " (D3DERR_OUTOFVIDEOMEMORY)"},
		{D3DERR_TOOMANYOPERATIONS,           " (D3DERR_TOOMANYOPERATIONS)"},
		{D3DERR_UNSUPPORTEDALPHAARG,         " (D3DERR_UNSUPPORTEDALPHAARG)"},
		{D3DERR_UNSUPPORTEDALPHAOPERATION,   " (D3DERR_UNSUPPORTEDALPHAOPERATION)"},
		{D3DERR_UNSUPPORTEDCOLORARG,         " (D3DERR_UNSUPPORTEDCOLORARG)"},
		{D3DERR_UNSUPPORTEDCOLOROPERATION,   " (D3DERR_UNSUPPORTEDCOLOROPERATION)"},
		{D3DERR_UNSUPPORTEDFACTORVALUE,      " (D3DERR_UNSUPPORTEDFACTORVALUE)"},
		{D3DERR_UNSUPPORTEDTEXTUREFILTER,    " (D3DERR_UNSUPPORTEDFACTORVALUE)"},
		{D3DERR_WASSTILLDRAWING,             " (D3DERR_WASSTILLDRAWING)"},
		{D3DERR_WRONGTEXTUREFORMAT,          " (D3DERR_WRONGTEXTUREFORMAT)"},
		{E_FAIL,                             " (E_FAIL)"},
		{E_INVALIDARG,                       " (E_INVALIDARG)"},
		{E_NOINTERFACE,                      " (E_NOINTERFACE)"},
		{E_NOTIMPL,                          " (E_NOTIMPL)"},
		{E_OUTOFMEMORY,                      " (E_OUTOFMEMORY)"},
		{S_NOT_RESIDENT,                     " (S_NOT_RESIDENT)"},
		{S_RESIDENT_IN_SHARED_MEMORY,        " (S_RESIDENT_IN_SHARED_MEMORY)"},
		{S_PRESENT_MODE_CHANGED,             " (S_PRESENT_MODE_CHANGED)"},
		{S_PRESENT_OCCLUDED,                 " (S_PRESENT_OCCLUDED)"},
		{D3DERR_UNSUPPORTEDOVERLAY,          " (D3DERR_UNSUPPORTEDOVERLAY)"},
		{D3DERR_UNSUPPORTEDOVERLAYFORMAT,    " (D3DERR_UNSUPPORTEDOVERLAYFORMAT)"},
		{D3DERR_CANNOTPROTECTCONTENT,        " (D3DERR_CANNOTPROTECTCONTENT)"},
		{D3DERR_UNSUPPORTEDCRYPTO,           " (D3DERR_UNSUPPORTEDCRYPTO)"},
		{D3DERR_PRESENT_STATISTICS_DISJOINT, " (D3DERR_PRESENT_STATISTICS_DISJOINT)"}
	};

	auto it = strings.find(code);

	if (it == strings.end())
		return msg;

	return msg + (*it).second;
}