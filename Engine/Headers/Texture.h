#pragma once

#include "ShaderProgram.h"

namespace engine
{
	///////////////////////////////////////////////////////////////////////
	class Texture;
	class Texture2D;
	class RenderTargetTexture;
	class TextureShadowMap;
	class CubeMap;
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

	class Quad2D;
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
	///////////////////////////////////////////////////////////////////////
	class CubeMap : public Texture
	{
	public:
		virtual void bind() override;
		virtual void unbind() override;
		void load(const std::vector<std::string>& filenames);
	};

	///////////////////////////////////////////////////////////////////////
	class Quad2D
	{
	public:
		static const GLuint VERTICES = 0;
		static const GLuint TEXCOORDS = 1;

		Quad2D();
		~Quad2D();

		//void draw(const Vec2& postions, const Vec2& size, const Texture& texture, const float tilingFactor, );
		void draw();

	private:
		GLuint _vao, _vbo;
		GLfloat _vertices[24];
	};
	/////////////////////////////////////////////////////////////////////// RenderTargetTexture
	class RenderTargetTexture : public Texture
	{
	public:
		RenderTargetTexture();
		~RenderTargetTexture();

		virtual void bind() override;
		virtual void unbind() override;

		void create(const int width, const int height);
		void clearColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a);


		void bindFramebuffer();
		void unbindFramebuffer();

		void renderQuad(ShaderProgram* shader, const std::string& textureUniform);

	private:
		Quad2D* _quad;
		GLuint _framebuffer, _rboDepthStencil;
		GLfloat _r, _g, _b, _a;
		int _width, _height;
	};
	/////////////////////////////////////////////////////////////////////// TextureShadowMap
	class TextureShadowMap : public Texture
	{
	public:
		TextureShadowMap();
		~TextureShadowMap();

		virtual void bind() override;
		virtual void unbind() override;
		void create(const int width, const int height);

		void bindFramebuffer();
		void unbindFramebuffer();

		void renderQuad(ShaderProgram* shader, const std::string& textureUniform);

	private:
		Quad2D* _quad;
		GLuint _framebuffer;
		int _width, _height;
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