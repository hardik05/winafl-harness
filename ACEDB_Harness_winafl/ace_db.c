/*
ACEDB fuzz harness for winafl.
Author: Hardik Shah
email: hardik05@gmail.com
web: http://hardik05.wordpress.com
note: you will get very slow speed with this.try to modify the code if you can.
*/
#include <stdio.h>
#include <tchar.h>
#include "acedao.tlh"

int FuzzMe(TCHAR* name)
{
	CoInitialize(NULL);
	_DBEnginePtr dbe;
	HRESULT hr = dbe.CreateInstance("DAO.DBEngine.120");
	if (hr==0x0)
	{
		DatabasePtr db = dbe->OpenDatabase(name);
		db->Close();
		db = NULL;
		dbe = NULL;
	}
	else
	{
		printf ("not able to createInstance,%0X", hr);
	}
	CoUninitialize();
	return 0;
}


int _tmain(int argc, TCHAR* argv[])
{
	if (argc < 2) {
		printf("Usage: %s <DB file>\n", (char *)argv[0]);
		return 0;
	}	
	FuzzMe(argv[1]);
	return 0;	
}