#include "ProcessHandling.h"
#include "HardCodedData.h"

int openProcess(char* command, STARTUPINFO	*startinfo, PROCESS_INFORMATION *procinfo)
{
	
	if (!CreateProcessA(NULL, command, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, startinfo, procinfo))
	{
		printf("Error occured during the creation of process\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

int closeProcess(HANDLE *hProcess)
{
	if (!CloseHandle(*hProcess))
	{
		printf("Error occured during the closing process\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
	}
	return 0;
}
