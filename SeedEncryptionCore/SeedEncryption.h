#pragma once
#ifdef DLLEXPORT
#define DLLTYPE extern "C" __declspec(dllexport)
#else
#define DLLTYPE extern "C" __declspec(dllimport)
#endif
#include "Include.h"

typedef int(*UserCallBack)(int funcnum, int seqeuncenum, char* data1, char* data2);

DLLTYPE int libCallbackTest();

DLLTYPE void Encryption(const char* inputpath, int inputlen, const char* outputpath, int outputlen);
DLLTYPE void Decryption(const char* inputpath, int inputlen, const char* outputpath, int outputlen);

LPVOID PrintLastError();

int SEED_CBC_PKCS5_ENCRYPT(char cKeyValue[16],
	char cInitValue[16],
	char* pInput,
	int nInputLength,
	char* pOutput);

int SEED_CBC_PKCS5_DECRYPT(char cKeyValue[16],
	char cInitValue[16],
	char* pInput,
	int nInputLength,
	char* pOutput);