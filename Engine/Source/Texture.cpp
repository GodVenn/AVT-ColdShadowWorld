#include "..\Headers\pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Texture.h"
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
}