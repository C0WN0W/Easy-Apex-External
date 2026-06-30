#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include "cheat.hpp"

using namespace std;
HWND hwnd = 0;

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleTitleA("Easy Apex External");

	Global::ScreenSize = { static_cast<float>(GetSystemMetrics(SM_CXSCREEN)),static_cast<float>(GetSystemMetrics(SM_CYSCREEN)) };
	Global::SightCenter = { Global::ScreenSize.x / 2.f,Global::ScreenSize.y / 2.f };

	InitializeCommunication();
	GetModuleBaseAddress(GetCurrentProcessId());


	while (!GamePid)
	{
		cout << "Waiting game process..." << endl;
		Sleep(2000);
		GamePid = getprocessid(L"r5apex_dx12.exe");
	}
	system("cls");

	const int maxAttempts = 10;
	bool success = false;
	for (int attempt = 0; attempt < maxAttempts; ++attempt) {
		GetModuleBaseAddress(GamePid);
		if (GameBase != 0) {
			success = true;
			break;
		}
		if (attempt < maxAttempts - 1) {
			Sleep(1000);
		}
	}
	if (!success) {
		cout << "Failed to get module address." << endl;
		system("pause");
		exit(0);
	}
	Global::GameBase = GameBase;
	
	GetProcessCr3(GamePid);

	cout << "[+] PID: " << GamePid << endl;
	cout << "[+] Base: " << hex << Global::GameBase << endl;

	while (!hwnd)
	{
		hwnd = FindWindowA("Respawn001", "Apex Legends");
		Sleep(2000);
	}
	cout << "[+] HWND: " << hwnd << endl;

	try
	{
		std::thread matrixTh(Cheat::MatrixUpdater);
		SetThreadPriority(matrixTh.native_handle(), THREAD_PRIORITY_TIME_CRITICAL);
		matrixTh.detach();

		std::thread worker(Cheat::WorkerThread);
		worker.detach();

		Gui.AttachAnotherWindow("Apex Legends", "Respawn001", Cheat::Run);
	}
	catch (OSImGui::OSException& e)
	{
		cout << e.what() << endl;
	}

	Cheat::running = false;
	Cheat::taskCV.notify_all();
}