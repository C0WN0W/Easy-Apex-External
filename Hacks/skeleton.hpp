#pragma once
#include "../Overlay/Draw.hpp"
#include "../Entity/Entity.hpp"

namespace Skeleton
{
	Vector3 BoneToScreen(uintptr_t entityPtr, Matrix m, int BoneID);
	void DebugDrawBones(uintptr_t entityPtr, const Matrix& m);
	void DrawSkeleton(uintptr_t entityPtr, Matrix& m, ImColor col);
}