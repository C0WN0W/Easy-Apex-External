#pragma once
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>

#include "Overlay/Draw.hpp"
#include "Overlay/Menu.hpp"
#include "Entity/Entity.hpp"
#include "Entity/structs.hpp"

namespace Cheat
{
	extern std::atomic<bool> running;

	Matrix GetViewMatrix();

	void Run();
	void WorkerThread();
	void MatrixUpdater();

	ImVec4 CalcRect(Player entity, Matrix m);
	bool IsBoxValid(ImVec4 box);
}
