



#ifndef UTILITY_H
#define UTILITY_H

#include <Windows.h>
#include <stdio.h>
#include <io.h>

#define READ_MODE	0x0001
#define WRITE_MODE	0x0002

FILE* RunShellCommand(const TCHAR* command, const int mode);

#endif




