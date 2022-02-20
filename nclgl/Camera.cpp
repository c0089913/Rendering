#include "Camera.h"
#include "Window.h"
#include <algorithm>
#include <fstream>

float clip(float n, float lower, float upper) {
	return std::max(lower, std::min(n, upper));
}

void Camera::UpdateCamera(float dt) {

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
	{
		m_UseControlPoints = !m_UseControlPoints;
	}
	if (m_UseControlPoints)
		AutoMove(dt);
	else
		InputFeedback(dt);
}

void Camera::InputFeedback(float dt)
{
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}
	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	float speed = 100.0f * dt;
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= forward * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position -= right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position += right * speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += speed;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= speed;
	}

	position.x = clip(position.x, 50.0f, 2600.0f);
	position.y = clip(position.y, 280.0f, 1150.0f);
	position.z = clip(position.z, -350.0f, 3800.0f);
}

void Camera::AutoMove(float dt)
{
	float speed = 100.0f * dt;
	Vector3 finalPos = m_ControlPoints[m_ActiveControlPoint];
	Vector3 direction =finalPos - position;
	position.y = finalPos.y;
	if (direction.Length() < speed)
	{
		m_ActiveControlPoint = (m_ActiveControlPoint + 1) % m_ControlPoints.size();
	}

	direction.Normalise();	
	position += direction * speed;
}

Matrix4 Camera::BuildViewMatrix() {

	if (m_UseControlPoints)
	{
		return Matrix4::BuildViewMatrix(position, m_LookAts[m_ActiveControlPoint]);
	}
	else
	{
		return Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
			Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
			Matrix4::Translation(-position);
	}
}


