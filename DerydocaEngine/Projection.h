#pragma once
#include "Transform.h"
#include "glm/glm.hpp"
#include "glm/geometric.hpp"

using namespace glm;

/* Types of projection modes */
enum ProjectionMode {
	/* Orthographic projection */
	Orthographic,
	/* Perspective projection */
	Perspective
};

struct Projection
{
public:
	mat4 getInverseViewProjectionMatrix(mat4 transformModelMatrix) const { return m_projectionMatrix * inverse(transformModelMatrix); }
	mat4 getViewMatrix(mat4 transformModelMatrix) { return inverse(transformModelMatrix); }
	mat4 getProjectionMatrix() { return m_projectionMatrix; }
	inline glm::mat4 getViewProjectionMatrix(mat4 transformModelMatrix) const { return m_projectionMatrix * transformModelMatrix; }

	/*
	Gets a matrix representing the camera's rotation and projection.

	@return Rotation projection matrix
	*/
	mat4 getRotationProjection(fquat quaternionRotation) const { return m_projectionMatrix * inverse(mat4_cast(quaternionRotation)); }

	void setProjectionMode(ProjectionMode mode) { m_projectionMode = mode; }

	void recalculateProjectionMatrix()
	{
		switch (m_projectionMode)
		{
		case ProjectionMode::Orthographic:
			m_projectionMatrix = glm::mat4(); //glm::ortho(-m_orthoSize, m_orthoSize, -m_orthoSize, m_orthoSize, m_zNear, m_zFar);
			break;
		case ProjectionMode::Perspective:
			m_projectionMatrix = glm::perspective(m_fov * (glm::pi<float>() / 180), m_aspect, m_zNear, m_zFar);
			break;
		default:
			m_projectionMatrix = glm::mat4();
			break;
		}
	}

	void setFov(float fov) { m_fov = fov; }
	void setAspectRatio(float aspectRatio) { m_aspect = aspectRatio; }
	void setAspectRatio(int width, int height) { m_aspect = (float)width / (float)(height != 0 ? height: 1); }
	void setZNear(float zNear) { m_zNear = zNear; }
	void setZFar(float zFar) { m_zFar = zFar; }

	float getFov() const { return m_fov; }
	float getAspectRatio() const { return m_aspect; }
	float getZNear() const { return m_zNear; }
	float getZFar() const { return m_zFar; }
private:
	mat4 m_projectionMatrix;
	ProjectionMode m_projectionMode = Perspective;
	float m_fov;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};