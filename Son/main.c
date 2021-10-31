#include "HardCodedData.h"
#include "FileHandling.h"

void Encrypt(char* plain, char* key, char* cypher)
{
	for (int i = 0; i < BUFFSIZE; i++)
	{
		cypher[i] = plain[i] ^ key[i];
	}
}

int exitCode(HANDLE hfiles[], int handlesNum)
{
	int exitCode = 0;
	for (int i = 0; i < handlesNum; i++)
	{
		exitCode |= closeFile(&hfiles[i]);
	}
	return exitCode;
}

int main(int argc, char* argv[])
{
	HANDLE plaintextFile = NULL;	// handle to the plain text file
	HANDLE keyFile = NULL;			// handle to the key text file
	HANDLE encryptedMessageFile = NULL;	// handle to the plain text file
	HANDLE hfiles[3] = { 0 }; //array of the handles to use when exitting the code

	char plainTxt[BUFFSIZE + 1] = { 0 }, key[BUFFSIZE + 1] = { 0 }, cypherTxt[BUFFSIZE + 1] = { 0 };
	
	//create handles to all files
	if (openFile(&plaintextFile, argv[1], READ))
	{
		//first file failed
		return(1);
	}
	hfiles[0] = plaintextFile;
	if (openFile(&keyFile, argv[3], READ))
	{
		//first file succeeded, second file failed
		return exitCode(hfiles, 1);
	}
	hfiles[1] = keyFile;
	if (openFile(&encryptedMessageFile, "Encrypted_message.txt", WRITE))
	{
		//first and second file succeeded, third file failed
		return exitCode(hfiles, 2);
	}
	hfiles[2] = encryptedMessageFile;

	//move file pointer to asked offset
	if (MoveFilePointer(&plaintextFile, atoi(argv[2])))
	{
		//exit
		return exitCode(hfiles, 3);
	}
	if (MoveFilePointer(&encryptedMessageFile, atoi(argv[2])))
	{
		//exit
		return exitCode(hfiles, 3);
	}

	//read text from plain text file.
	if (ReadFromFile(plaintextFile, plainTxt, BUFFSIZE))
	{
		//exit
		return exitCode(hfiles, 3);
	}
	
	//read key from key file
	if (ReadFromFile(keyFile, key, BUFFSIZE))
	{
		//exit
		return exitCode(hfiles, 3);
	}

	//encrypt data
 	Encrypt(plainTxt, key, cypherTxt);

	
	//write encrypted message to dest file
	if (WriteToFile(encryptedMessageFile, cypherTxt, strlen(cypherTxt)))
	{
		return exitCode(hfiles, 3);
	}

	//close all file handles and exit
	return exitCode(hfiles, 3);
}
