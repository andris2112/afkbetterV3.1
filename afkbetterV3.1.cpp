#include <iostream>
#include <Windows.h>
#include <thread>

// String: $MPPLY_CASINO_CHIPS_WON_GD
// Value: 0

void ThrowError(const char* Error)
{
	const auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);

	printf(Error, "\n");

	std::this_thread::sleep_for(std::chrono::seconds(3));
	TerminateProcess(GetCurrentProcess(), 0);

	return;
}

void ScanForExit()
{
	printf("Press F2 to stop the bot!\n");

	while (TRUE)
	{
		if (GetAsyncKeyState(VK_F2))
		{
			TerminateProcess(GetCurrentProcess(), 0);
		}
	}

	return;
}

BOOLEAN PressEnter()
{
	auto Input = INPUT();
	RtlZeroMemory(&Input, sizeof(Input));

	char Buffer[120] = { 0 };

	GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_ILANGUAGE, Buffer, sizeof(Buffer));
	const auto hKeyboardLayout = LoadKeyboardLayoutA(Buffer, KLF_ACTIVATE);

	const auto EnterVk = VkKeyScanExA('\n', hKeyboardLayout);
	const auto EnterVKey = MapVirtualKeyA(LOBYTE(EnterVk), 0);

	Input.type = INPUT_KEYBOARD;
	Input.ki.dwFlags = KEYEVENTF_SCANCODE;
	Input.ki.wScan = EnterVKey;

	SendInput(1, &Input, sizeof(Input));
	RtlZeroMemory(&Input, sizeof(Input));

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	Input.type = INPUT_KEYBOARD;
	Input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	Input.ki.wScan = EnterVKey;

	SendInput(1, &Input, sizeof(Input));
	RtlZeroMemory(&Input, sizeof(Input));

	return TRUE;
}

BOOLEAN MouseClick()
{
	auto Input = INPUT();
	RtlZeroMemory(&Input, sizeof(Input));

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

	SendInput(1, &Input, sizeof(Input));
	RtlZeroMemory(&Input, sizeof(Input));

	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(1, &Input, sizeof(Input));
	RtlZeroMemory(&Input, sizeof(Input));

	return TRUE;
}

int main()
{
	printf("Press F1 to start the bot!\n");

	while (!GetAsyncKeyState(VK_F1))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	const auto GameWindow = FindWindowA("grcWindow", "Grand Theft Auto V");
	const auto MenuWindow = FindWindowA(NULL, "5$ bronze pack recovery");

	if (GameWindow == NULL)
	{
		ThrowError("Couldn't find the game window, aborting...");
		return -1;
	}

	if (MenuWindow == NULL)
	{
		ThrowError("Couldn't find the menu window, aborting...");
		return -1;
	}

	printf("Game window: 0x%p\n", GameWindow);
	printf("Menu window: 0x%p\n", MenuWindow);

	const auto hScanForExitThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ScanForExit, 0, 0, 0);

	if (hScanForExitThread == NULL)
	{
		ThrowError("Failed to create exit thread, aborting...");
		return -1;
	}
	else
	{
		CloseHandle(hScanForExitThread);
	}

	while (TRUE)
	{
		SetForegroundWindow(GameWindow);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		PressEnter();

		SetForegroundWindow(MenuWindow);
		SetWindowPos(MenuWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE);

		std::this_thread::sleep_for(std::chrono::milliseconds(250));

		SetCursorPos(100, 525);
		MouseClick();
		MouseClick();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}