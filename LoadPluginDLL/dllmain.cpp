#include "pch.h"
#include <stdio.h>
#include <Windows.h>
#define _CRT_SECURE_NO_WARNINGS


static void Console()
{
    AllocConsole();
    freopen("conin$", "r", stdin);
    freopen("conout$", "w", stdout);
    freopen("conout$", "w", stderr);
    HWND consoleHandle = GetConsoleWindow();
    MoveWindow(consoleHandle, 1, 1, 680, 480, 1);
}

void LoadPlugins()
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WCHAR exePath[MAX_PATH];
	WCHAR directoryPath[MAX_PATH];
	WCHAR fullPath[MAX_PATH];

	// Get the full path of the executable
	GetModuleFileName(NULL, exePath, MAX_PATH);

	WCHAR* lastSlash = wcsrchr(exePath, L'\\');
	if (lastSlash)
	{
		*lastSlash = 0;  // Null-terminate the string at the last backslash
	}

	// Prepare the search pattern "*.plugindll" in the directory
	swprintf(directoryPath, MAX_PATH, L"%s\\*.plugindll", exePath);


	hFind = FindFirstFile(directoryPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("No plugins found.\n");
		return;
	}

	do
	{
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			// Construct the full path to the DLL
			WCHAR fullPath[MAX_PATH];
			swprintf(fullPath, MAX_PATH, L"%s\\%s", exePath, ffd.cFileName);

			// Load the DLL
			printf("Loading Plugin: %ls\n", fullPath);
			HMODULE hModule = LoadLibrary(fullPath);
			if (hModule == NULL) {
				printf("Failed to load: %ls\n", fullPath);
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	// Close the handle
	FindClose(hFind);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		Console();
		LoadPlugins();
	
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

