#pragma once
#include "geometry/Pos3F.h"
#include "geometry/Vec3F.h"
class Node {
public:
	Node();
	Node(Pos3F pos);
	Pos3F pos;
	Vec3F vel;
	Pos3F last_pos;
};