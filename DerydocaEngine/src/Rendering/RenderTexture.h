#pragma once
#include "Rendering\Texture.h"
#include "Rendering\RenderingMode.h"

namespace DerydocaEngine::Rendering
{

	class RenderTexture : public Texture
	{
	public:
		RenderTexture();
		RenderTexture(RenderingMode renderingMode);
		~RenderTexture();

		void bind(unsigned int const& unit);
		void bindAsRenderTexture();
		float getAspectRatio();
		void initializeTexture(int const& width, int const& height);
	private:
		void createRenderbuffer(int textureUnit, GLenum format, unsigned int & textureId);
		void createRenderbuffers_Forward();
		void createRenderbuffers_Deferred();
		void destroyGraphicsApiObjects();
	private:
		RenderingMode m_renderingMode;
		unsigned int m_framebuffer;
		std::vector<unsigned int> m_renderBufferIds;
	};

}