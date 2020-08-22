/*
winafl harness to fuzz wmf2emf conversion
Author: Hardik Shah
Email: hardik05@gmail.com
Web: http://hardik05.wordpress.com
*/

#include <windows.h>
#include <iostream>
#include <vector>

#pragma pack(1)
typedef struct tagWIN16RECT
{
	WORD left;
	WORD top;
	WORD right;
	WORD bottom;
} WIN16RECT;

typedef struct tagPLACEABLEMETAHEADER
{
	DWORD key;
	WORD hmf;
	WIN16RECT bbox;
	WORD inch;

	DWORD reserved;
	WORD checksum;
} PLACEABLEMETAHEADER;
#pragma pack()



HENHMETAFILE  WINAPI FuzzMe(IN LPCWSTR lpszMetaFile)
{
	HANDLE hFile = ::CreateFileW(
		lpszMetaFile,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == NULL || hFile == INVALID_HANDLE_VALUE)
		return NULL;

	DWORD dwSize = ::GetFileSize(hFile, NULL);
	std::vector<BYTE> data(dwSize);

	DWORD dwRead;
	BOOL bSuccess = ::ReadFile(hFile, &data[0], dwSize, &dwRead, NULL);
	::CloseHandle(hFile);

	HENHMETAFILE hEnhMetaFile = NULL;

	if (bSuccess)
	{
		PLACEABLEMETAHEADER * hdr = (PLACEABLEMETAHEADER*)&data[0];
		int iPlaceableHeaderSize = sizeof(PLACEABLEMETAHEADER);

		int iOffset = 0;
		if (hdr->key != 0x9AC6CDD7)  //not placeable header
		{
			iOffset = 0;  //offset remains zero
		}
		else
		{
			iOffset = iPlaceableHeaderSize; //file is offset with placeable windows metafile header
		}

		hEnhMetaFile = ::SetWinMetaFileBits(data.size(), &data[iOffset], NULL, NULL);
		return 0;
	}

	return 0;
}

int main()
{
	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList == NULL)
	{
		return 1;
	}

	HENHMETAFILE hEMF = FuzzMe(szArgList[1]);

	LocalFree(szArgList);

	return 0;
}
