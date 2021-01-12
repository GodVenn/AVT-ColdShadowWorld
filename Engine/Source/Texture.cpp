#include "..\Headers\pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Texture.h"
#include "..\Headers\App.h"
#include <stb_image.h>


namespace engine
{

	/////////////////////////////////////////////////////////////////////// TextureInfo
	TextureInfo::TextureInfo(GLenum unit, GLuint index, const std::string& uniform, Texture* texture, Sampler* sampler)
		:unit(unit), index(index), uniform(uniform), texture(texture), sampler(sampler)
	{
	}
	/// NOTE: Shader need to be bind before call
	void TextureInfo::updateShader(ShaderProgram* shader)
	{
		glActiveTexture(unit);
		texture->bind();
		shader->setUniform1i(uniform, index);
		if (sampler)
		{
			sampler->bind(unit);
		}
	}
	/////////////////////////////////////////////////////////////////////// Texture
	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}
	//////////////////////////////////////////////////////////////////////
	void Texture2D::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void Texture2D::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Texture2D::load(const std::string& filename)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(filename.c_str(), &width, &height, &channels, 4);
		}

		std::string failMsg = "Failed to load 2D Texture from path: " + filename;
		ASSERT(data, failMsg);

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureStorage2D(id, 1, GL_RGBA8, width, height);

		// Default sampler
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

	}
	/////////////////////////////////////////////////////////////////////// RENDER TARGET TEXTURE
	RenderTargetTexture::RenderTargetTexture()
		: _framebuffer(-1), _rboDepthStencil(-1), _r(0.f), _g(0.f), _b(0.f), _a(1.f), _width(0), _height(0)
	{
		_quad = new Quad2D();
	}
	RenderTargetTexture::~RenderTargetTexture()
	{
		glDeleteFramebuffers(1, &_framebuffer);
		glDeleteTextures(1, &id);
		glDeleteTextures(1, &_rboDepthStencil);

		delete _quad;
	}
	void RenderTargetTexture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void RenderTargetTexture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void RenderTargetTexture::create(const int width, const int height)
	{
		_width = width;
		_height = height;
		glGenFramebuffers(1, &_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

		// Create color texture
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach colorTexture with the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);

		// Create depth and stencil texture
		glGenTextures(1, &_rboDepthStencil);
		glBindTexture(GL_TEXTURE_2D, _rboDepthStencil);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);

		// Attach depth and stencil texture with the framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _rboDepthStencil, 0);

		ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "Framebuffer is not complete!");



		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void RenderTargetTexture::clearColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a)
	{
		_r = r, _g = g, _b = b, _a = a;
	}
	void RenderTargetTexture::bindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		glViewport(0, 0, _width, _height);
		glClearColor(_r, _g, _b, _a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	void RenderTargetTexture::unbindFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void RenderTargetTexture::renderQuad(ShaderProgram* shader, const std::string& textureUniform)
	{
		shader->bind();
		shader->setUniform1i(textureUniform, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);

		glDisable(GL_DEPTH_TEST);
		_quad->draw();
		glEnable(GL_DEPTH_TEST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	/////////////////////////////////////////////////////////////////////// SAMPLERs
	Sampler::Sampler()
	{
		glCreateSamplers(1, &samplerId);
	}

	Sampler::~Sampler()
	{
		glDeleteSamplers(1, &samplerId);
	}
	void Sampler::bind(GLuint unit)
	{
		glBindSampler(unit, samplerId);
	}

	void Sampler::unbind(GLuint unit)
	{
		glBindSampler(unit, 0);
	}
	/////////////////////////////////////////////////////////////////////// 
	void NearestSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	/////////////////////////////////////////////////////////////////////// 
	void LinearSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	/////////////////////////////////////////////////////////////////////// 
	void NearestMipmapNearestSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	/////////////////////////////////////////////////////////////////////// 
	void NearestMipmapLinearSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}
	/////////////////////////////////////////////////////////////////////// 
	void LinearMipmapLinearSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	/////////////////////////////////////////////////////////////////////// 
	void LinearMipmapNearestSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	}
	///////////////////////////////////////////////////////////////////////
	void LinearAnisotropicSampler::create()
	{
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	}
	/////////////////////////////////////////////////////////////////////// 
	void MaxAnisotropicSampler::create()
	{
		GLfloat max = 0.f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max);
		glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glSamplerParameteri(samplerId, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<GLuint>(max));
		//DEBUG
		/** /
		std::cout << "Sampler set to maximum ansiotropy: " << max << std::endl;
		/**/
	}
	/////////////////////////////////////////////////////////////////////// Quad2D
	const GLfloat data[] = {
	1.f, -1.0f, 1.f, 0.f,
	1.f,  1.f,  1.f, 1.f,
   -1.f,  -1.f,  0.f, 0.f,
   -1.f, 1.f,  0.f, 1.f
	};
	Quad2D::Quad2D()
	{
		// Fill the data
		memcpy(_vertices, data, sizeof(data));

		// Vertex creation
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), &_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(TEXCOORDS);
		glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		glBindVertexArray(0);
	}
	Quad2D::~Quad2D()
	{
		glDeleteBuffers(1, &_vao);
		glDeleteVertexArrays(1, &_vao);
	}
	void Quad2D::draw()
	{
		glBindVertexArray(_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}