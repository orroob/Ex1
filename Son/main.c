#include "HardCodedData.h"
#include "FileHandling.h"

/// <summary>
/// This function encrypts a plain text by xoring it with a given key, and puts the result in a given buffer.
/// </summary>
/// <param name="plain"> - a 16 bytes long plain text char*.</param>
/// <param name="key"> - a 16 bytes long char* key.</param>
/// <param name="cypher"> - a char* object to which the result is written.</param>
void Encrypt(char* plain, char* key, char* cypher)
{
	for (int i = 0; i < BUFFSIZE; i++)
	{
		cypher[i] = plain[i] ^ key[i];
	}
}

/// <summary>
/// This function closes all open handles before exiting the code.
/// </summary>
/// <param name="hfiles"> - a HANDLEs array, holding 1-3 handles to be closed.</param>
/// <param name="handlesNum"> - the number of handles to be closed.</param>
/// <returns>Returns 1 if failed, 0 otherwise.</returns>
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
	// check arguments are valid
	if (argc != 4)
	{
		printf("Arguments Error\n");
		return 1;
	}
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
		return 1 || exitCode(hfiles, 1);
	}
	hfiles[1] = keyFile;
	if (openFile(&encryptedMessageFile, "Encrypted_message.txt", WRITE))
	{
		//first and second file succeeded, third file failed
		return 1 || exitCode(hfiles, 2);
	}
	hfiles[2] = encryptedMessageFile;

	//move file pointer to asked offset
	if (MoveFilePointer(&plaintextFile, atoi(argv[2]), READ, "plain.txt"))
	{
		//exit
		return 1 || exitCode(hfiles, 3);
	}
	if (MoveFilePointer(&encryptedMessageFile, atoi(argv[2]), WRITE, "enc.txt"))
	{
		//exit
		return 1 || exitCode(hfiles, 3);
	}
	
	//read text from plain text file.
	if (ReadFromFile(plaintextFile, plainTxt, BUFFSIZE))
	{
		//exit
		return 1 || exitCode(hfiles, 3);
	}
	
	//read key from key file
	if (ReadFromFile(keyFile, key, BUFFSIZE))
	{
		//exit
		return 1 || exitCode(hfiles, 3);
	}

	//encrypt data
 	Encrypt(plainTxt, key, cypherTxt);

	
	//write encrypted message to dest file
	if (WriteToFile(encryptedMessageFile, cypherTxt, strlen(cypherTxt)))
	{
		return 1 || exitCode(hfiles, 3);
	}

	//close all file handles and exit
	return exitCode(hfiles, 3);
}
