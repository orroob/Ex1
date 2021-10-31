#include "HardCodedData.h"
#include "FileHandling.h"

void Encrypt(char* plain, char* key, char* cypher)
{
	for (int i = 0; i < BUFFSIZE; i++)
	{
		cypher[i] = plain[i] ^ key[i];
	}
}

int main(int argc, char* argv[])
{
	HANDLE plaintextFile = NULL;	// handle to the plain text file
	HANDLE keyFile = NULL;			// handle to the key text file
	HANDLE encryptedMessageFile = NULL;	// handle to the plain text file

	char plainTxt[BUFFSIZE + 1] = { 0 }, key[BUFFSIZE + 1] = { 0 }, cypherTxt[BUFFSIZE + 1] = { 0 };
	
	//create handles to all files
	if (openFile(&plaintextFile, argv[1], READ))
	{
		//first file failed
		return(1);
	}
	
	if (openFile(&keyFile, argv[3], READ))
	{
		//first file succeeded, second file failed
		closeFile(&plaintextFile);
		return(1);
	}
	if (openFile(&encryptedMessageFile, "Encrypted_message.txt", WRITE))
	{
		//first and second file succeeded, third file failed
		closeFile(&plaintextFile);
		closeFile(&keyFile);
		return(1);
	}

	int offset = atoi(argv[2]);

	//read text from plain text file.
	if (ReadFromFile(plaintextFile, plainTxt, BUFFSIZE))
	{
		//exit
		closeFile(&plaintextFile);
		return(1);
	}
	
	//read key from key file
	if (ReadFromFile(keyFile, key, BUFFSIZE))
	{
		//exit
		closeFile(&plaintextFile);
		return(1);
	}

	//encrypt data
	Encrypt(plainTxt, key, cypherTxt);

	//move file pointer to asked offset
	MoveFilePointer(&encryptedMessageFile, atoi(argv[2]));

	//write encrypted message to dest file
	WriteToFile(encryptedMessageFile, cypherTxt, strlen(cypherTxt));

	//close all file handles
	if (closeFile(&plaintextFile))
	{
		closeFile(&keyFile);
		closeFile(&encryptedMessageFile);
		return(1);
	}
	if (closeFile(&keyFile))
	{
		closeFile(&encryptedMessageFile);
		return(1);
	}
	if (closeFile(&encryptedMessageFile))
	{
		return(1);
	}
	return(0);
}
