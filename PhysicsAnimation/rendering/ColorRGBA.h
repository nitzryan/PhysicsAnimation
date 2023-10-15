#pragma once

struct ColorRGBA {
	float r;
	float g;
	float b;
	float a;

	ColorRGBA(float red, float green, float blue, float alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	void Add(const ColorRGBA& c) {
		r += c.r;
		g += c.g;
		b += c.b;
		if (r > 1)
			r = 1;
		if (g > 1)
			g = 1;
		if (b > 1)
			b = 1;
	}

	const static ColorRGBA RED() { return ColorRGBA(1.0, 0, 0, 1.0); }
	const static ColorRGBA BLUE() { return ColorRGBA(0, 0., 1., 1.); }
	const static ColorRGBA GREEN() { return ColorRGBA(0., 1., 0., 1.); }
	const static ColorRGBA YELLOW() { return ColorRGBA(1., 1., 0., 1.); }
	const static ColorRGBA PURPLE() { return ColorRGBA(1., 0., 1., 1.); }
	const static ColorRGBA CYAN() { return ColorRGBA(0., 1., 1., 1.); }
	const static ColorRGBA SILVER() { return ColorRGBA(0.753, 0.753, 0.753, 1); }
};
