#include <Windows.h>

int APIENTRY WinMain([[maybe_unused]]HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR lpCmdLine, [[maybe_unused]] int nCmdShow)
{
	MessageBox(NULL, "Hello, world!", "Hello, world!", MB_OK);
	return 0;
}