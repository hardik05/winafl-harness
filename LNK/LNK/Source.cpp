/*
LNK file harness to fuzz with winafl.
Author: hardik shah
Email: hardik05@gmail.com
web: http://hardik05.wordpress.com
code taken from MSDN.
*/

#include "windows.h"
#include "shobjidl.h"
#include "shlguid.h"
#include "strsafe.h"
#include "shlobj.h"



HRESULT FuzzMe(WCHAR *wsz)
{
	IPersistFile* ppf;
	IShellLink* psl;
	HRESULT hres;
	IShellLinkDataList *iShellLinkDataList;	
			
	CoInitialize(NULL);
		
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);

		if (SUCCEEDED(hres))
		{
			hres = psl->QueryInterface(IID_IShellLinkDataList, (LPVOID*)&iShellLinkDataList);			
			hres = ppf->Load((LPCOLESTR)wsz, STGM_READ);

			if (SUCCEEDED(hres))
			{
				// Resolve the link. 
				hres = psl->Resolve(NULL, SLR_NO_UI);
			}
			ppf->Release();	
			psl->Release();
		}
		CoUninitialize();
	
	}
	
	return hres;
}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WCHAR wsz[MAX_PATH];
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
	{
		return 1;
	}

	if (argCount != 2) {
		printf("Usage: %s <path to .lnk file>\n", szArgList[0]);
		return 1;
	}
	FuzzMe(szArgList[1]);
	return 0;
}

