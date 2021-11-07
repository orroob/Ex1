#include "FileHandlingFather.h"
#include "HardCodedDataFather.h"


void CreateProcessSimpleMain(char* txt_file_name, DWORD offset, char* key_file_name)
{


	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	TCHAR				command[] = { NULL };
	sprintf_s(command, "./Son.exe %s %d %s", txt_file_name, offset, key_file_name);
	/* TCHAR is a win32
	generic char which may be either a simple (ANSI) char or a unicode char,
	depending on behind-the-scenes operating system definitions. Type LPTSTR
	is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */
	/*  Start the child process. */
	retVal = CreateProcessSimple(command, &procinfo);


	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return;
	}


	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		INFINITE); /* Waiting 5 secs for the process to end */

	printf("WaitForSingleObject output: ");
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	default:
		printf("0x%x\n", waitcode);
	}

	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	{
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(
			procinfo.hProcess,
			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate,
					note the above command may also fail, so another WaitForSingleObject is required.
					We skip this for brevity */
	}

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	printf("The exit code for the process is 0x%x\n", exitcode);

	/* Note: process is still being tracked by OS until we release handles */
	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
}



BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION* ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(
		NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}
/// <summary>
/// This function creates a file using winAPI's CreateFileA function.
///
/// ASSUMPTIONS: 
///		access mode is either GENERIC_WRITE or GENERIC_READ.
///		share mode is 0 (no share).
///		creation disposition is either OPEN_ALWAYS or OPEN_EXISTING, depending on the access mode.
/// </summary>
/// <param name="hfile"> - a HANDLE* object to which the function will place the created file handle. should be initiated to NULL.</param>
/// <param name="fileName"> - a char* object containing the file's name.</param>
/// <param name="accessMode"> - desired access mode to the file. 0-read; 1-write</param>
/// <returns>returns 0 if secceeded, 1 otherwise</returns>
int OpenFile(HANDLE* hfile, char* fileName, int accessMode)
{

	*hfile = CreateFileA(fileName, (accessMode) ? GENERIC_WRITE : GENERIC_READ, 0, NULL, (accessMode) ? OPEN_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (*hfile == INVALID_HANDLE_VALUE)
	{

		printf("Error occured during the creation/opening of %s\n", fileName);
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}

/// <summary>
/// This function writes data to a file using winAPI's WriteFile function. Failure can be caused by WriteFile failure, or when the written data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to the file that is written to.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer) after the read was done).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int WriteToFile(HANDLE hfile, char* buffer, int messageLen)
{
	DWORD writtenMassageLen;
	if (!WriteFile(hfile, buffer, messageLen, &writtenMassageLen, NULL))
	{
		printf("Error occured during the writing to file \n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	if (writtenMassageLen != messageLen)
	{
		printf("Error occured during the writing to the file \n");
		wchar_t buf[256];
		printf("Not all of the data was written successfully\n");
		return 1;
	}
	return 0;
}

/// <summary>
/// This function reads data from a file and places it to a buffer, using winAPI's ReadFile function. Failure can be caused by ReadFile failure, or when the read data's length isn't equal to the given messageLen.
/// </summary>
/// <param name="hfile"> - a HANDLE object to the file that is read.</param>
/// <param name="buffer"> - a char* object containing the data that will be written.</param>
/// <param name="messageLen"> - an int containing the message's length. (should be equal to strlen(buffer)).</param>
/// <returns> The function returns 0 if succeeded, 1 otherwise.</returns>
int ReadFromFile(HANDLE hfile, char* buffer, int messageLen)
{
	DWORD readMassageLen;
	if (!ReadFile(hfile, buffer, messageLen - 1, &readMassageLen, NULL))
	{
		printf("Error occured during the reading from file \n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	buffer[messageLen] = '\0';

	if (readMassageLen != messageLen - 1)
	{
		printf("Error occured during the reading from the file \n");
		wchar_t buf[256];
		printf("Not all of the data was read successfully\n");
		return 1;
	}
	return 0;
}

/// <summary>
/// This function receives a pointer to a handle and closes it, using winAPI's CloseHandle function. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile"> - pointer to a handle.</param>
/// <returns> returns 0 if secceeded, 1 otherwise. </returns>
int CloseFile(HANDLE* hfile)
{
	if (!CloseHandle(*hfile))
	{
		printf("Error occured during the closing file\n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
	}

	return 0;
}

/// <summary>
/// This function receives a pointer to a handle and an int, then moves the file pointer with the givven offset using winAPI's SetFilePointer function. if an error occured, prints the GetLastError() value.
/// </summary>
/// <param name="hfile"> - a HANDLE* to the file.</param>
/// <param name="offset"> - the offset given to the file pointer.</param>
/// <returns>Returns 0 if succeeded, 1 otherwise.</returns>
int MoveFilePointer(HANDLE* hfile, int offset)
{
	DWORD writtenMassageLen;
	if (!SetFilePointer(hfile, (long)offset, NULL, FILE_BEGIN))
	{
		printf("Error occured during the writing to file \n");
		wchar_t buf[256];
		printf("Error code: %d\n", FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL));
		return 1;
	}
	return 0;
}