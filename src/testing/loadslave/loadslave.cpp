#include <windows.h>

int main()
{
	try
	{
		// Chargement normal de la DLL
		HMODULE hModule = LoadLibrary("slave_dll.dll");
		//HMODULE hModule = LoadLibrary("isinjected.dll");
		Sleep(INFINITE);
	}
	catch(...)
	{
	}
	return 0;
}
