#pragma once
#include <windows.h>
#include <process.h>


#define FINDER_IS_FILE 32
#define FINDER_IS_FOLDER 16
#define FINDER_IS_HIDENFOLDER 18
enum CHARACTER_ENCODING
{
	ANSI,
	Unicode,
	Unicode_big_endian,
	UTF8_with_BOM,
	UTF8_without_BOM
};

#include "SEED_KISA.h"
#include <fstream>

using namespace std;

