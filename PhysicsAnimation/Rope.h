#include <vector>
#include "./geometry/Pos3F.h"
#include "./geometry/Vec3F.h"
#include "rendering/ColorRGBA.h"

class Rope{
public:
	// dir is unit vector that points in the direction of the rope
	Rope(int length, float link_len, Pos3F start, Vec3F dir);
	~Rope();
	void Relax_Step(float dt);
	void Render();
	void Update(float dt, Vec3F gravity);
	int relax_setps = 1;

private:
	int length;
	float link_len;
	std::vector<Pos3F> position;
	std::vector<Pos3F> last_position;
	std::vector<Vec3F> velocity;
	float time;
};