//---------------------------------------------------------------
//winafl harness to fuzz mdb files. this resulted in 12+ cves.
//Author: hardik shah
//mail: hardik05@gmail.com
//Twitter: @hardik05
//---------------------------------------------------------------

#include <afx.h>

int FuzzMe(TCHAR* name)
{
	CDaoDatabase db;
	//printf("parsing file:%s", (char*)name);

	if (name != NULL) {
			db.Open(name);
		  db.Close();
	}
	return(0);
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

