#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"

//Default constructor
Camera::Camera()
{
	m_Type = "Camera";
	m_LookAt = vec3(0.0f, 0.0f, 0.0f);
	m_Up = vec3(0.0f, 1.0f, 0.0f);
	m_NearClip = 0.1f;
	m_FarClip = 100.0f;
	m_FOV = 45.0f;
	m_AspectRatio = 16.0f / 9.0f;
	m_View = mat4();
	m_Projection = mat4();
}

//Destructor
Camera::Camera()
{
}

void Camera::update()
{
	//Get camera's position from it's parent's transform
	vec3 position = m_Parent->getTransform->getPosition();

	m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);

}


