#pragma once

#include "../geometry/Pos3F.h"
#include "../gameObjects/ColorRGBA.h"

class VBOWriter {
public:
	VBOWriter() = delete;

	static void DrawTriangle(
		float** vboPointer,
		const Pos3F& p0,
		const Pos3F& p1,
		const Pos3F& p2,
		const ColorRGBA& color
	) {
		AddPoint(vboPointer, p0, color);
		AddPoint(vboPointer, p1, color);
		AddPoint(vboPointer, p2, color);
	}

	static void DrawCircle(
		float** vboPointer,
		const Pos3F& center,
		float radius,
		int numSegments,
		const ColorRGBA& color
	) {
		float thetaStep = 2 * 3.141592 / numSegments;
		float theta = 0;
		for (int i = 0; i < numSegments; i++) {
			float x = center.x + radius * cos(theta);
			float y = center.y + radius * sin(theta);

			*(*vboPointer) = x;
			(*vboPointer)++;
			*(*vboPointer) = y;
			(*vboPointer)++;
			*(*vboPointer) = color.r;
			(*vboPointer)++;
			*(*vboPointer) = color.g;
			(*vboPointer)++;
			*(*vboPointer) = color.b;
			(*vboPointer)++;
			*(*vboPointer) = color.a;
			(*vboPointer)++;

			theta += thetaStep;

			x = center.x + radius * cos(theta);
			y = center.y + radius * sin(theta);

			*(*vboPointer) = x;
			(*vboPointer)++;
			*(*vboPointer) = y;
			(*vboPointer)++;
			*(*vboPointer) = color.r;
			(*vboPointer)++;
			*(*vboPointer) = color.g;
			(*vboPointer)++;
			*(*vboPointer) = color.b;
			(*vboPointer)++;
			*(*vboPointer) = color.a;
			(*vboPointer)++;

			*(*vboPointer) = center.x;
			(*vboPointer)++;
			*(*vboPointer) = center.y;
			(*vboPointer)++;
			*(*vboPointer) = color.r;
			(*vboPointer)++;
			*(*vboPointer) = color.g;
			(*vboPointer)++;
			*(*vboPointer) = color.b;
			(*vboPointer)++;
			*(*vboPointer) = color.a;
			(*vboPointer)++;
		}
	}

	static void AddPoint(float** vboPointer,
		const Pos3F& p, const ColorRGBA& color) {
		*(*vboPointer) = p.x;
		(*vboPointer)++;
		*(*vboPointer) = p.y;
		(*vboPointer)++;
		*(*vboPointer) = color.r;
		(*vboPointer)++;
		*(*vboPointer) = color.g;
		(*vboPointer)++;
		*(*vboPointer) = color.b;
		(*vboPointer)++;
		*(*vboPointer) = color.a;
		(*vboPointer)++;
	}
};