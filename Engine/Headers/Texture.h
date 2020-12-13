#pragma once

#include "ShaderProgram.h"

namespace engine
{
	///////////////////////////////////////////////////////////////////////
	class Texture;
	class Texture2D;
	struct TextureInfo;

	class Sampler;
	class NearestSampler;
	class LinearSampler;
	class NearestMipmapNearestSampler;
	class NearestMipmapLinearSampler;
	class LinearMipmapNearestSampler;
	class LinearMipmapLinearSampler;
	class LinearAnisotropicSampler;
	class MaxAnisotropicSampler;
	/////////////////////////////////////////////////////////////////////// TextureInfo
	struct TextureInfo
	{
		GLenum unit;
		GLuint index;
		std::string uniform;
		Texture* texture = nullptr;
		Sampler* sampler = nullptr;

		TextureInfo(GLenum unit, GLuint index, const std::string& uniform, 
			Texture* texture, Sampler* sampler = nullptr);
		void updateShader(ShaderProgram* shader);
	};
	/////////////////////////////////////////////////////////////////////// TEXTUREs
	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture();

		virtual void bind() = 0;
		virtual void unbind() = 0;
	protected:
		GLuint id = 0;
	};
	///////////////////////////////////////////////////////////////////////
	class Texture2D : public Texture
	{
	public:
		virtual void bind() override;
		virtual void unbind() override;
		void load(const std::string& filename);
	};

	/////////////////////////////////////////////////////////////////////// SAMPLERs
	class Sampler
	{
	public:
		Sampler();
		virtual ~Sampler();

		virtual void create() = 0;

		void bind(GLuint unit);
		void unbind(GLuint unit);
	protected:
		GLuint samplerId;
	};
	///////////////////////////////////////////////////////////////////////
	class NearestSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class LinearSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class NearestMipmapNearestSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class NearestMipmapLinearSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class LinearMipmapNearestSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class LinearMipmapLinearSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class LinearAnisotropicSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
	///////////////////////////////////////////////////////////////////////
	class MaxAnisotropicSampler : public Sampler
	{
	public:
		virtual void create() override;
	};
}