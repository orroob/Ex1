#include "HardCodedDataFather.h"
#include "FileHandlingFather.h"


int main(int argc, char* argv[]){	
	HANDLE *hFile = NULL;
	PLARGE_INTEGER file_size = NULL;
	OpenFile(hFile, argv[1], GENERIC_READ);
	GetFileSizeEx(hFile, file_size);
	CloseFile(hFile);
	DWORD offset = 0;
	for (DWORD iter = 0; iter < (DWORD)file_size / 16; iter++) {
		CreateProcessSimpleMain(argv[1] ,offset, argv[2]);
		offset += 16;
	}
	return 0;

}


