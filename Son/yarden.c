#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#define BUFSIZE 16


void ReadToBuffer(LPCSTR text, int offset, LPCSTR string_to_encrypt) {

	HANDLE hFile = CreateFileA(text,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,// is needed?
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		exit(1);
	}
	DWORD dwBytesToRead = BUFSIZE;
	DWORD dwBytesRead = 0; // number of bytes that were read
	BOOL retValFile = ReadFile(
		hFile,
		(void*)string_to_encrypt,
		dwBytesToRead,
		&dwBytesRead,
		(void*)offset);
	CloseHandle(hFile);
	if (dwBytesRead != dwBytesToRead) {
		exit(1);
	}
	return 0; //valid?


}

void WriteToFile(LPCSTR EncryptedMessage, int offset) {
	HANDLE hFileEncrypted = CreateFileA(
		"Encrypted_message.txt",
		GENERIC_WRITE,
		0,//PROBLEM?
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFileEncrypted == INVALID_HANDLE_VALUE) {
		exit(1);
	}
	DWORD dwBytesWritten = 0;
	DWORD dwBytesToWrite = BUFSIZE;
	WriteFile(hFileEncrypted,
		(void*)EncryptedMessage,
		dwBytesToWrite,
		&dwBytesWritten,
		(void*)offset);
	CloseHandle(hFileEncrypted);
	if (dwBytesWritten == dwBytesToWrite) {
		return 0; //success
	}
	else {
		exit(1); //unsuccessful
	}
}


void main1(int argc, char* argv[]) {

	TCHAR string_to_encrypt[BUFSIZE] = { NULL };
	ReadToBuffer(argv[0], atoi(argv[1]), string_to_encrypt); //read next section to buffer
	TCHAR key[BUFSIZE] = { NULL };
	ReadToBuffer(argv[2], 0, key); //read key to buffer
	TCHAR encrypted[BUFSIZE] = { NULL };
	for (int i = 0; i<=BUFSIZE - 1; i++) {// encryption
		encrypted[i] = string_to_encrypt[i] ^ key[i];
	}
	WriteToFile(encrypted, atoi(argv[1]));//write to file
	exit(0);
}