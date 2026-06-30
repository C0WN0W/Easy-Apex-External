#include "skeleton.hpp"

struct ScreenBone {
	// bool valid;
	ImVec2 pos;
};

Vector3 Skeleton::BoneToScreen(uintptr_t entityPtr, Matrix m, int BoneID)
{
	Vector3 BonePos = NewHitbox(entityPtr, BoneID);
	Vector3 ScreenPos;
	world_to_screen(BonePos, m, Global::ScreenSize.x, Global::ScreenSize.y, ScreenPos);
	return ScreenPos;
}

void Skeleton::DebugDrawBones(uintptr_t entityPtr, const Matrix& m)
{
	constexpr int BoneCount = 30;
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	std::vector<ImVec2> usedPositions;

	for (int i = 0; i < BoneCount; i++) {
		Vector3 screenPos = BoneToScreen(entityPtr, m, i);
		ImVec2 pos(screenPos.x, screenPos.y);

		bool overlap = false;
		for (auto& p : usedPositions) {
			if ((fabs(pos.x - p.x) < 2.0f && fabs(pos.y - p.y) < 2.0f)) {
				overlap = true;
				break;
			}
		}
		ImU32 color = overlap ? IM_COL32(0, 255, 0, 255) : IM_COL32(255, 255, 0, 255);
		ImVec2 drawPos = overlap ? ImVec2(pos.x + 10, pos.y) : pos;

		char buf[16];
		snprintf(buf, sizeof(buf), "%d", i);
		drawList->AddText(ImVec2(drawPos.x + 5, drawPos.y), color, buf);
	}
}

void Skeleton::DrawSkeleton(uintptr_t entityPtr, Matrix& m, ImColor col)
{
	// Found 28-32 valid bone in DebugDrawBones()
	// You may find fingers bone in bigger loop like 128
	constexpr int BoneCount = 30;
	ScreenBone screenBones[BoneCount];

	for (int i = 0; i < BoneCount; i++)
	{
		Vector3 screenPos = BoneToScreen(entityPtr, m, i);
		if (screenPos.x <= 0 || screenPos.x > Global::ScreenSize.x ||
			screenPos.y <= 0 || screenPos.y > Global::ScreenSize.y)
		{
			return;
		}

		screenBones[i].pos = ImVec2(screenPos.x, screenPos.y);
	}

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	auto draw_bone = [&](int BoneID1, int BoneID2)
	{
		int i1 = static_cast<int>(BoneID1);
		int i2 = static_cast<int>(BoneID2);
		drawList->AddLine(screenBones[i1].pos, screenBones[i2].pos, col, 1.5f);
	};

	// Body
	draw_bone(0, 1); // Head - Neck
	draw_bone(1, 2); // Neck - Chest
	draw_bone(2, 3); // Chest - Stomach
	draw_bone(3, 4); // Stomach - Pelvis

	// Right hand
	draw_bone(1, 9); // Neck - Shoulder (R)
	draw_bone(9, 10); // Shoulder - Elbow (R)
	draw_bone(10, 11); // Elbow - Hand (R)

	// Left hand
	draw_bone(1, 6); // Neck - Shoulder (L)
	draw_bone(6, 7); // Shoulder - Elbow (L)
	draw_bone(7, 8); // Elbow - Hand (L)

	// Right leg
	draw_bone(4, 16); // Pelvis - Thigh (R)
	draw_bone(16, 17); // Thigh - Knee (R)
	draw_bone(17, 18); // Knee - Calve (R)
	//draw_bone(18, 19); // Calve - Foot (R)

	// Left leg
	draw_bone(4, 12); // Pelvis - Thigh (L)
	draw_bone(12, 13); // Thigh - Knee (L)
	draw_bone(13, 14); // Knee - Calve (L)
	//draw_bone(14, 15); // Calve - Foot (L)
}