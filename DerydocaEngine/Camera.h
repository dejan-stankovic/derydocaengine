#pragma once

#include "Transform.h"
#include "GameComponent.h"
#include "GameObject.h"
#include "Skybox.h"
#include "Color.h"
#include "Material.h"
#include "RenderTexture.h"
#include "Display.h"
#include "Rectangle.h"

class MatrixStack;

/*
Object used for rendering the world to screen.
*/
class Camera : public GameComponent
{
public:
	GENINSTANCE(Camera);

	/* Types of screen clear methods when rendering a camera. */
	enum ClearMode {
		/* Do not clear the screen. */
		NoClear,

		/* Clears the screen with a solid color. */
		ColorClear,

		/* Clears the screen with a skybox. */
		SkyboxClear
	};

	/* Types of projection modes */
	enum ProjectionMode {
		/* Orthographic projection */
		Orthographic,
		/* Perspective projection */
		Perspective
	};

	enum RenderingMode {
		Forward = 0,
		Deferred = 1
	};

	Camera();
	Camera(float fov, float aspect, float zNear, float zFar);
	~Camera();

	/*
	Gets a matrix representing the camera's projection.

	@return VP matrix
	*/
	inline glm::mat4 getInverseViewProjectionMatrix() const { return m_projectionMatrix * glm::inverse(m_transform->getModel()); }

	inline glm::mat4 getViewProjectionMatrix() const { return m_projectionMatrix * m_transform->getModel(); }

	inline glm::mat4 getProjectionMatrix() const { return m_projectionMatrix; }

	inline glm::mat4 getViewMatrix() const { return glm::inverse(m_transform->getModel()); }
	
	inline RenderTexture* getRenderTexture() const { return m_renderTexture; }

	/*
	Gets a matrix representing the camera's rotation and projection.

	@return Rotation projection matrix
	*/
	inline glm::mat4 getRotationProjection() const { return m_projectionMatrix * glm::inverse(glm::mat4_cast(m_transform->getQuat())); }
	
	/*
	Sets the field of view for this camera

	@fov Field of view in degrees
	*/
	void setFov(float fov);

	/*
	Sets the clear mode

	@clearMode Clear mode to switch to
	*/
	void setClearMode(ClearMode clearMode) { m_clearMode = clearMode; }

	/*
	Sets the clear color

	@clearColor Color to use when clearing the camera
	*/
	void setClearColor(Color clearColor) { m_clearColor = clearColor; }

	/*
	Sets the skybox material

	@skyboxMaterial Material to use for the skybox
	*/
	void setSkybox(Material* skyboxMaterial) { m_skyboxMaterial = skyboxMaterial; }

	/*
	Renders the root node through this camera

	@root Root game object to render all of its children
	*/
	void renderRoot(GameObject* root);

	void setDisplay(Display* display) { m_display = display; }
	void setRenderTexture(RenderTexture* renderTexture) { m_renderTexture = renderTexture; }
	void init();
	void setDisplayRect(float x, float y, float w, float h);
	void setRenderingMode(RenderingMode mode);

	void createGBufTex(GLenum textureUnit, GLenum format, GLuint &texid, int width, int height);

	void setProjectionMode(ProjectionMode mode);
	void setOrthoSize(float size);
	float getOrthoSize(float size) { return m_orthoSize; }
	void deserialize(YAML::Node node);
	Shader* getPostProcessShader() const { return m_postProcessShader; }
private:
	float m_fov, m_aspect, m_zNear, m_zFar;
	glm::mat4 m_projectionMatrix;
	Transform* m_transform;
	Color m_clearColor;
	Skybox* m_skybox;
	ClearMode m_clearMode = NoClear;
	ProjectionMode m_projectionMode = Perspective;
	RenderingMode m_renderingMode;
	Material* m_skyboxMaterial;
	MatrixStack* m_matrixStack;
	RenderTexture* m_renderTexture;
	Display* m_display;
	Rectangle* m_displayRect;
	Mesh* m_quad;
	Shader* m_postProcessShader;
	float m_orthoSize = 10.0f;
	GLuint m_deferredFBO;
	GLuint m_gbuffDepth, m_gbuffPos, m_gbuffNorm, m_gbuffColor;
	Shader* m_deferredRendererCompositor;

	void clear();
	inline void recalcPerspectiveMatrix();
	void setIdentityMatricies(Shader* shader);
};

