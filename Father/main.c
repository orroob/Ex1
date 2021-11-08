#define _CRT_SECURE_NO_WARNINGS

#include "HardCodedData.h"
#include "ProcessHandling.h"
#include "FileHandling.h"

int main(int argc, char* argv[])
{
	// check arguments are valid
	if (argc != 3)
	{
		printf("Arguments Error Father\n");
		return 1;
	}

	HANDLE plaintextFile = NULL;
	int file_size;

	// chaeck the plain text file's size
	if (openFile(&plaintextFile, argv[1], READ))
	{
		//opening file failed
		return(1);
	}
	if ((file_size = GetFileSize(plaintextFile, NULL)) == INVALID_FILE_SIZE)
	{
		// GetFileSize failed
		closeFile(&plaintextFile);
		return 1;
	}
	if (closeFile(&plaintextFile))
	{
		// closing of file failed
		return 1;
	}

	char* plain_text_file_name = argv[1];
	char* key_file_name = argv[2];
	char command[500] = { 0 };
	
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 };
	PROCESS_INFORMATION procinfo;

	// pass through the plain text file and encrypt every 16 bytes
	for (int i = 0; i < file_size/16; i++)
	{
		sprintf(command, "Son.exe %s %d %s", plain_text_file_name, i*16, key_file_name);

		// start the process
		if (openProcess(command, &startinfo, &procinfo))
		{
			return (1);
		}

		if (procinfo.hProcess == NULL)
		{
			// process handle is NULL
			printf("Error occured while running a process\n");
			closeProcess(&(procinfo.hProcess));
			closeProcess(&(procinfo.hThread));
			return 1;
		}

		// wait for process to finish
		if (WaitForSingleObject(procinfo.hProcess, INFINITE) == WAIT_FAILED)
		{
			printf("Waiting for the son failed (%d).\n", GetLastError());
			closeProcess(&(procinfo.hProcess));
			closeProcess(&(procinfo.hThread));
			return 1;
		}

		// check if process has exited correctly
		DWORD exitcode = 0;
		if (!GetExitCodeProcess(procinfo.hProcess, &exitcode))
		{
			printf("Error getting the exit code - %d\n", GetLastError());
			if (closeProcess(&(procinfo.hProcess)) || closeProcess(&(procinfo.hThread)))
			{
				return 1;
			}
		}

		// close process handles
		if (closeProcess(&(procinfo.hProcess)) || closeProcess(&(procinfo.hThread)))
		{
			// closing process handles failed
			printf("Error occured while closing process handles\n");
			return 1;
		}
	}
	return 0;
}

