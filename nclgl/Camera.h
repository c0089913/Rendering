#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include <vector>
class Camera {

	std::vector<Vector3> m_ControlPoints;
	std::vector<Vector3> m_LookAts;
	int m_ActiveControlPoint;
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		m_ActiveControlPoint = 0;
		m_UseControlPoints = false;
	};

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
		m_ActiveControlPoint = 0;
		m_UseControlPoints = false;
	}

	~Camera(void) {};

	void UpdateCamera(float dt = 1.0f);

	void InputFeedback(float dt);

	void AutoMove(float dt);

	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetYaw() const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }

	void AddControlPoint(Vector3 position, Vector3 lookAt) { m_ControlPoints.push_back(position); m_LookAts.push_back(lookAt); }

	bool m_UseControlPoints;
protected:
	float yaw;
	float pitch;
	Vector3 position; // Set to 0 ,0 ,0 by Vector3 constructor ;)
};
