#pragma once
#define DLLEXPORT
#include "SeedEncryption.h"

// 암호화 키값
unsigned char g_cKeyValue[16] = { 0x88, 0xE3, 0x4F, 0x8F, 0x08, 0x17, 0x79, 0xF1, 0xE9, 0xF3, 0x94, 0x37, 0x0A, 0xD4, 0x05, 0x89 };
// 암호화 초기 키값
unsigned char g_cInitValue[16] = { 0x26, 0x8D, 0x66, 0xA7, 0x35, 0xA8, 0x1A, 0x81, 0x6F, 0xBA, 0xD9, 0xFA, 0x36, 0x16, 0x25, 0x01 };
// 폴더 열어서 폴더구조 똑같이 만들고 하부 cpp파일 암호화
// 입출력 폴더 선택

UserCallBack callback = NULL;

DLLTYPE int libCallbackTest()
{
	if (callback != NULL)
		callback(10, 10, NULL, NULL);
	return 0;
}

DLLTYPE void Encryption(const char* inputpath, int inputlen, const char* outputpath, int outputlen)
{
	char chBuff[10000] = { 0, };					// 입력 버퍼
	char chFilePath[_MAX_PATH] = { 0, };			// .encryption 파일 경로 버퍼
	char chData[10000] = { 0, };					// 출력 버퍼
	char chInfoBuf[10000] = { 0, };					// .encryption 파일 출력 버퍼
	char chDstFileName[_MAX_PATH] = { 0, };			// 암호화 출력 파일 명 (확장자 포함)
	char chDstFileTitle[_MAX_PATH] = { 0, };		// 암호화 출력 파일 타이틀 (확장자 제외)
	char chTmpbuf[4] = { 0x0d, 0x00, 0x0a, 0x00 };	// 개행 코드 (\r\n)

	int Len = 0;
	int i = 0, nShift = 0;
	unsigned int nTotalSize = outputlen;

	// 입력용 파일 Open, Text Mode
	ifstream infile(inputpath);
	// 출력용 파일 Open, Bin Mode
	ofstream outstream(outputpath, ios::binary);
	// 입력 파라메터에서 파일명 파싱
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
	// 암호화시 사용한 파일 길이 저장 파일 쓰기용 Open
	sprintf_s(chFilePath, "%s\\%s.ect", chFilePath, chDstFileTitle);
	ofstream outstream2(chFilePath, ios::binary);

	// 파일 열기 예외 처리
	if (!infile.is_open() || !outstream.is_open() || !outstream2.is_open())
	{
		PrintLastError();
		return;
	}

	try {
		while (infile.getline(chBuff, 10000))	// 라인 별로 읽어서 암호화
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

			// 암호화
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
	char chBuff[10000] = { 0, };					// 입력 버퍼
	char chFilePath[_MAX_PATH] = { 0, };			// .encryption 파일 경로 버퍼
	strcpy_s(chFilePath, inputpath);				// 입력 경로 복사
	char chData[10000];								// 출력 버퍼
	char chInfoBuf[10000] = { 0x00, };				// .encryption 파일 출력 버퍼
	char chDstFileName[_MAX_PATH] = { 0, };			// 암호화 출력 파일명 (확장자 포함)
	char chDstFileTitle[_MAX_PATH] = { 0, };		// 암호화 출력 타이틀 (확장자 제외)
	char chTmpbuf[4] = { 0x0d, 0x00, 0x0a, 0x00 };	// 개행 코드 (\r\n)
	int Len = 0;
	int i = 0;
	int nEncrytionLen = 0;
	unsigned int nTotalSize = 0;

	// 입력용 파일 Open, Bin Mode
	ifstream infile(inputpath, ios::binary);
	// 출력용 파일 Open, Bin Mode
	ofstream outstream(outputpath, ios::binary);
	// 입력 파라메터에서 파일명 파싱
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
	// 암호화시 사용한 파일 길이 저장파일 읽기용 Open
	sprintf_s(chFilePath, "%s\\%s.ect", chFilePath, chDstFileTitle);
	ifstream infile2(chFilePath, ios::binary);

	// 파일 열기 예외 처리
	if (!infile.is_open() || !outstream.is_open() || !infile2.is_open())
	{
		PrintLastError();
		return;
	}
	try {
		while (infile2.getline(chInfoBuf, 10000))	// 암호화시 사용한 파일 길이 저장파일 라인 별 읽기
		{
			nEncrytionLen = atoi(chInfoBuf);		// 암호화시 저장한 길이 변환
			infile.read(chBuff, nEncrytionLen);		// 길이 만큼 파일 읽기

			// 복호화
			Len = SEED_CBC_PKCS5_DECRYPT((char*)g_cKeyValue, (char*)g_cInitValue, chBuff, nEncrytionLen, chData);
			outstream.write(chData, Len);			// 복호화된 코드를 파일에 출력
			
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

// 에러 출력 함수
// 리스트 박스 전용
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

// 암호화 함수
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
	char  cPadding;         /* Padding 초기값 */

	unsigned char  cText[100000] = { 0, };       /* 원시 데이타 */
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

	//    Padding Size 계산
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

	//    암호화 전 CBC 방식 설정
	for (i = 0; i < SeedBlockSize; i++)
	{
		outputBlock[i] = cInitValue[i];
	}

	//     Seed 암호화
	for (k = 0; k < BlockCount; k++)
	{
		int index;
		index = k * SeedBlockSize;
		memcpy(inputBlock, cText + index, SeedBlockSize);
		//      CBC 운영 모드
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

// 복호화 함수
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

	unsigned char cText[10000]; // 원시 데이타
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

	// 암호화 전 CBC 방식 설정
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
		// CBC 운영 모드
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