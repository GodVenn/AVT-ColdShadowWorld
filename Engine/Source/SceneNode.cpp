#include "../Headers/pch.h"
#include "../Headers/SceneNode.h"
#include "../Headers/Base.h"

namespace engine
{
	SceneNode::SceneNode()
	{
		this->parent = nullptr;
		this->mesh = nullptr;
		this->shader = nullptr;
		this->callback = nullptr;
		this->matrix = MatFactory::createIdentityMat4();

	}

	SceneNode::SceneNode(SceneNode* parent)
	{
		this->parent = parent;
		this->mesh = nullptr;
		this->shader = nullptr;
		this->callback = nullptr;
		this->matrix = MatFactory::createIdentityMat4();

	}

	SceneNode::~SceneNode()
	{
		for (auto& n : nodes)
		{
			delete n;

		}
		for (auto& tex : textures)
		{
			delete tex;

		}
		textures.clear();
		nodes.clear();
	}

	SceneNode* SceneNode::createNode()
	{
		SceneNode* node = new SceneNode(this);
		this->nodes.push_back(node);
		return node;

	}

	void SceneNode::addNode(SceneNode* node)
	{
		node->setParent(this);
		this->nodes.push_back(node);
	}

	void SceneNode::addTextureInfo(TextureInfo* texInfo)
	{
		this->textures.push_back(texInfo);
	}

	std::vector<SceneNode*> SceneNode::getNodes()
	{
		return this->nodes;
	}

	void SceneNode::setMesh(Mesh* mesh)
	{
		this->mesh = mesh;
	}

	Mesh* SceneNode::getMesh()
	{
		return this->mesh;
	}

	void SceneNode::setMatrix(const Mat4& matrix)
	{
		this->matrix = matrix;
	}

	void SceneNode::setParent(SceneNode* parent)
	{
		if (!this->parent)
		{
			this->parent = parent;
		}
	}

	void SceneNode::setCallback(ISceneNodeCallBack* callback)
	{
		this->callback = callback;
	}

	Mat4 SceneNode::getModelMatrix()
	{
		if (parent)
		{
			return this->parent->getModelMatrix() * this->matrix;
		}
		return this->matrix;
	}

	void SceneNode::setShaderProgram(ShaderProgram* shader)
	{
		this->shader = shader;
	}

	ShaderProgram* SceneNode::getActiveShaderProgram()
	{
		if (!shader)
		{
			if (parent)
			{
				shader = parent->getActiveShaderProgram();
			}
			else
			{
				ASSERT(false, "No shader was found for the scene node");
			}
		}

		return this->shader;
	}

	void SceneNode::draw()
	{
		ShaderProgram* activeShader = getActiveShaderProgram();

		for (auto& n : nodes)
		{
			n->draw();
		}

		if (callback)
		{
			callback->beforeDraw(this);
		}

		if (mesh)
		{
			activeShader->bind();
			if (!textures.empty())
			{
				for (auto& tex : textures)
				{
					tex->updateShader(activeShader);
				}
			}
			Mat4 modelMatrix = getModelMatrix();
			activeShader->setUniformMat4("ModelMatrix", modelMatrix);

			mesh->draw();

			activeShader->unbind();
		}

		if (callback)
		{
			callback->afterDraw(this);
		}

	}
}