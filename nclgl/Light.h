#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	Light() {} // Default constructor , we’ll be needing this later!
	Light(const Vector4& position, const Vector4& colour, float radius, float intensity) {
		this->position = position;
		this->colour = colour;
		this->radius = radius;
		this->intensity = intensity;
	}

	~Light(void) {};

	Vector4 GetPosition() const { return position; }
	void SetPosition(const Vector4& val) { position = val; }

	float GetRadius() const { return radius; }
	void SetRadius(float val) { radius = val; }

	float GetIntensity() const { return intensity; }
	void SetIntensity(float val) { intensity = val; }

	Vector4 GetColour() const { return colour; }
	void SetColour(const Vector4& val) { colour = val; }

protected:
	Vector4 position;
	float radius;
	Vector4 colour;
	float intensity;
};