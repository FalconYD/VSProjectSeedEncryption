#pragma once
#define DLLEXPORT
#include "SeedEncryption.h"

// ��ȣȭ Ű��
unsigned char g_cKeyValue[16] = { 0x88, 0xE3, 0x4F, 0x8F, 0x08, 0x17, 0x79, 0xF1, 0xE9, 0xF3, 0x94, 0x37, 0x0A, 0xD4, 0x05, 0x89 };
// ��ȣȭ �ʱ� Ű��
unsigned char g_cInitValue[16] = { 0x26, 0x8D, 0x66, 0xA7, 0x35, 0xA8, 0x1A, 0x81, 0x6F, 0xBA, 0xD9, 0xFA, 0x36, 0x16, 0x25, 0x01 };
// ���� ��� �������� �Ȱ��� ����� �Ϻ� cpp���� ��ȣȭ
// ����� ���� ����

UserCallBack callback = NULL;

DLLTYPE int libCallbackTest()
{
	if (callback != NULL)
		callback(10, 10, NULL, NULL);
	return 0;
}

DLLTYPE void Encryption(const char* inputpath, int inputlen, const char* outputpath, int outputlen)
{
	char chBuff[10000] = { 0, };					// �Է� ����
	char chFilePath[_MAX_PATH] = { 0, };			// .encryption ���� ��� ����
	char chData[10000] = { 0, };					// ��� ����
	char chInfoBuf[10000] = { 0, };					// .encryption ���� ��� ����
	char chDstFileName[_MAX_PATH] = { 0, };			// ��ȣȭ ��� ���� �� (Ȯ���� ����)
	char chDstFileTitle[_MAX_PATH] = { 0, };		// ��ȣȭ ��� ���� Ÿ��Ʋ (Ȯ���� ����)
	char chTmpbuf[4] = { 0x0d, 0x00, 0x0a, 0x00 };	// ���� �ڵ� (\r\n)

	int Len = 0;
	int i = 0, nShift = 0;
	unsigned int nTotalSize = outputlen;

	// �Է¿� ���� Open, Text Mode
	ifstream infile(inputpath);
	// ��¿� ���� Open, Bin Mode
	ofstream outstream(outputpath, ios::binary);
	// �Է� �Ķ���Ϳ��� ���ϸ� �Ľ�
	for (i = outputlen; i >= 0; i--)
	{
		if (outputpath[i] == '\\')
		{
			strncpy_s(chFilePath, outputpath, i);
			int nDstFileNameLen = outputlen - i;
			strncpy_s(chDstFileName, outputpath + i + 1, nDstFileNameLen + 2);
			for (int j = nDstFileNameLen; j >= 0; j--)
			{
				if (chDstFileName[j] == '.')
				{
					strncpy_s(chDstFileTitle, chDstFileName, j);
					break;
				}
			}
			break;
		}
	}
	// ��ȣȭ�� ����� ���� ���� ���� ���� ����� Open
	sprintf_s(chFilePath, "%s\\%s.ect", chFilePath, chDstFileTitle);
	ofstream outstream2(chFilePath, ios::binary);

	// ���� ���� ���� ó��
	if (!infile.is_open() || !outstream.is_open() || !outstream2.is_open())
	{
		PrintLastError();
		return;
	}

	try {
		while (infile.getline(chBuff, 10000))	// ���� ���� �о ��ȣȭ
		{
			if (chBuff[0] == 0x00)
				memcpy(&chBuff[0], &chBuff[1], 9999);
			for (i = 0; i < 10000; i++)
			{
				if (chBuff[i] == 0x0d)
					break;
			}
			if (i == 10000)
			{
				for (i = 0; i < 10000; i++)
				{
					if (chBuff[i] == 0x00)
						break;
				}
				if (i == 10000)
					i = 1;
				else
				{
					chBuff[i++] = 0x0a;
				}
			}
			else
			{
				memcpy(&chBuff[i], chTmpbuf, sizeof(char) * 4);
				i += 4;
			}
			memset(chData, 0x00, sizeof(chData));

			// ��ȣȭ
			Len = SEED_CBC_PKCS5_ENCRYPT((char*)g_cKeyValue, (char*)g_cInitValue, chBuff, i, chData);
			outstream.write(chData, Len);
			
			sprintf_s(chInfoBuf, "%d\r\n", Len);
			outstream2.write(chInfoBuf, strlen(chInfoBuf));
			
			memset(chBuff, 0x00, 10000);
		}
	}
	catch (...) {
		PrintLastError();
	}

	infile.close();
	outstream.close();
	outstream2.close();
}

DLLTYPE void Decryption(const char* inputpath, int inputlen, const char* outputpath, int outputlen)
{
	char chBuff[10000] = { 0, };					// �Է� ����
	char chFilePath[_MAX_PATH] = { 0, };			// .encryption ���� ��� ����
	strcpy_s(chFilePath, inputpath);				// �Է� ��� ����
	char chData[10000];								// ��� ����
	char chInfoBuf[10000] = { 0x00, };				// .encryption ���� ��� ����
	char chDstFileName[_MAX_PATH] = { 0, };			// ��ȣȭ ��� ���ϸ� (Ȯ���� ����)
	char chDstFileTitle[_MAX_PATH] = { 0, };		// ��ȣȭ ��� Ÿ��Ʋ (Ȯ���� ����)
	char chTmpbuf[4] = { 0x0d, 0x00, 0x0a, 0x00 };	// ���� �ڵ� (\r\n)
	int Len = 0;
	int i = 0;
	int nEncrytionLen = 0;
	unsigned int nTotalSize = 0;

	// �Է¿� ���� Open, Bin Mode
	ifstream infile(inputpath, ios::binary);
	// ��¿� ���� Open, Bin Mode
	ofstream outstream(outputpath, ios::binary);
	// �Է� �Ķ���Ϳ��� ���ϸ� �Ľ�
	for (i = inputlen; i >= 0; i--)
	{
		if (inputpath[i] == '\\')
		{
			strncpy_s(chFilePath, inputpath, i);
			int nDstFileNameLen = outputlen - i;
			strncpy_s(chDstFileName, inputpath + i + 1, nDstFileNameLen);
			for (int j = nDstFileNameLen; j >= 0; j--)
			{
				if (chDstFileName[j] == '.')
				{
					strncpy_s(chDstFileTitle, chDstFileName, j);
					break;
				}
			}
			break;
		}
	}
	// ��ȣȭ�� ����� ���� ���� �������� �б�� Open
	sprintf_s(chFilePath, "%s\\%s.ect", chFilePath, chDstFileTitle);
	ifstream infile2(chFilePath, ios::binary);

	// ���� ���� ���� ó��
	if (!infile.is_open() || !outstream.is_open() || !infile2.is_open())
	{
		PrintLastError();
		return;
	}
	try {
		while (infile2.getline(chInfoBuf, 10000))	// ��ȣȭ�� ����� ���� ���� �������� ���� �� �б�
		{
			nEncrytionLen = atoi(chInfoBuf);		// ��ȣȭ�� ������ ���� ��ȯ
			infile.read(chBuff, nEncrytionLen);		// ���� ��ŭ ���� �б�

			// ��ȣȭ
			Len = SEED_CBC_PKCS5_DECRYPT((char*)g_cKeyValue, (char*)g_cInitValue, chBuff, nEncrytionLen, chData);
			outstream.write(chData, Len);			// ��ȣȭ�� �ڵ带 ���Ͽ� ���
			
			memset(chBuff, 0x00, 10000);
		}
	}
	catch (...) {
		PrintLastError();
	}

	infile.close();
	infile2.close();
	outstream.close();
}

// ���� ��� �Լ�
// ����Ʈ �ڽ� ����
LPVOID PrintLastError()
{
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();
	DWORD error =
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0,
			NULL);
	return lpMsgBuf;
}

// ��ȣȭ �Լ�
int SEED_CBC_PKCS5_ENCRYPT(char cKeyValue[16],
	char cInitValue[16],
	char* pInput,
	int nInputLength,
	char* pOutput)
{
	int index = 0, i = 0, nEncryptLength = 0;
	int k = 0, j = 0;
	int nPaddingCount = 0;
	double BlockCount = 0;
	float fInputLength = 0.0;
	char  cPadding;         /* Padding �ʱⰪ */

	unsigned char  cText[100000] = { 0, };       /* ���� ����Ÿ */
	unsigned char  tempOutput[100000];
	DWORD dwRoundKey[SeedBlockSize * 2];

	unsigned char inputBlock[SeedBlockSize];
	unsigned char outputBlock[SeedBlockSize];

	//TRACE("SEED_CBC_PKCS5_ENCRYPT\n");

	//     Seed Round Key
	SeedRoundKey(dwRoundKey, (BYTE*)cKeyValue);

	memset(inputBlock, 0x00, SeedBlockSize);
	memset(outputBlock, 0x00, SeedBlockSize);

	memset(cText, 0x00, sizeof(cText));

	memcpy(cText, pInput, nInputLength);

	//    Padding Size ���
	cPadding = (char)(SeedBlockSize - (nInputLength % SeedBlockSize));
	//cPadding = (char)(nInputLength * 31 % SeedBlockSize);

	nPaddingCount = (int)cPadding;

	fInputLength = (float)nInputLength;
	BlockCount = ceil(fInputLength / SeedBlockSize);

	if (nPaddingCount == SeedBlockSize)
	{
		BlockCount = BlockCount + 1.0;
	}

	for (i = 0; i < nPaddingCount; i++)
	{
		cText[nInputLength + i] = cPadding;
	}

	//    ��ȣȭ �� CBC ��� ����
	for (i = 0; i < SeedBlockSize; i++)
	{
		outputBlock[i] = cInitValue[i];
	}

	//     Seed ��ȣȭ
	for (k = 0; k < BlockCount; k++)
	{
		int index;
		index = k * SeedBlockSize;
		memcpy(inputBlock, cText + index, SeedBlockSize);
		//      CBC � ���
		for (j = 0; j < SeedBlockSize; j++)
		{
			inputBlock[j] = (char)((inputBlock[j] ^ outputBlock[j]));
		}
		SeedEncrypt(inputBlock, dwRoundKey);
		memcpy(outputBlock, inputBlock, SeedBlockSize);
		memcpy(tempOutput + index, outputBlock, SeedBlockSize);
	}

	memcpy(pOutput, tempOutput, SeedBlockSize * k);
	return SeedBlockSize * k;
}

// ��ȣȭ �Լ�
int SEED_CBC_PKCS5_DECRYPT(char cKeyValue[16],
	char cInitValue[16],
	char* pInput,
	int nInputLength,
	char* pOutput)
{
	int    index = 0, i = 0, nEncryptLength = 0;
	int k = 0, j = 0;
	double BlockCount = 0;
	float fInputLength = 0.0;

	unsigned char cText[10000]; // ���� ����Ÿ
	unsigned char tempOutput[10000];
	DWORD dwRoundKey[SeedBlockSize * 2];

	unsigned char inputBlock[SeedBlockSize];
	unsigned char outputBlock[SeedBlockSize];
	unsigned char cipherBlock[SeedBlockSize];

	printf("SEED_CBC_PKCS5_DECRYPT\n");

	// Seed Round Key
	SeedRoundKey(dwRoundKey, (BYTE*)cKeyValue);

	memset(tempOutput, 0x00, sizeof(tempOutput[0]) * 10000);
	memset(inputBlock, 0x00, SeedBlockSize);
	memset(outputBlock, 0x00, SeedBlockSize);

	memset(cText, 0x00, sizeof(cText));

	memcpy(cText, pInput, nInputLength);

	fInputLength = (float)nInputLength;
	BlockCount = ceil(fInputLength / SeedBlockSize);

	// ��ȣȭ �� CBC ��� ����
	for (i = 0; i < SeedBlockSize; i++)
	{
		outputBlock[i] = cInitValue[i];
	}

	for (k = 0; k < BlockCount; k++)
	{
		int index;
		index = k * SeedBlockSize;
		memcpy(inputBlock, cText + index, SeedBlockSize);
		memcpy(cipherBlock, inputBlock, SeedBlockSize);

		SeedDecrypt(inputBlock, dwRoundKey);
		// CBC � ���
		for (j = 0; j < SeedBlockSize; j++)
		{
			inputBlock[j] = (char)((outputBlock[j] ^ inputBlock[j]));
		}
		memcpy(outputBlock, cipherBlock, SeedBlockSize);
		memcpy(tempOutput + index, inputBlock, SeedBlockSize);
	}

	memcpy(pOutput, tempOutput, SeedBlockSize * k);

	return SeedBlockSize * k - (int)inputBlock[SeedBlockSize - 1];
}