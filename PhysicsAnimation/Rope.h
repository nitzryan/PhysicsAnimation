#include <vector>
#include "./geometry/Pos3F.h"
#include "./geometry/Vec3F.h"
#include "rendering/ColorRGBA.h"

class Rope{
public:
	// dir is unit vector that points in the direction of the rope
	Rope(int length, float link_len, Pos3F start, Vec3F dir);
	void Update_pos(float dt, Vec3F gravity);
	void Update_vel(float dt);
	int relax_setps = 1;
	int get_length();
	std::vector<Pos3F> position;
	Pos3F get_base_pos();

private:
	int length;
	float link_len;
	std::vector<Pos3F> last_position;
	std::vector<Vec3F> velocity;
	float time;
	Pos3F base_pos;
};