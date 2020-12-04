#pragma once

#include "ShaderProgram.h"
#include "Mesh.h"

namespace engine 
{
	//////////////////////////////////////////////////////////////////////
	class SceneNode;
	class ISceneNodeCallBack;

	/////////////////////////////////////////////////////////////////////// SceneNodeCallback
	class ISceneNodeCallBack
	{
	public:
		virtual void beforeDraw(SceneNode* node) = 0;
		virtual void afterDraw(SceneNode* node) = 0;
	};

	/////////////////////////////////////////////////////////////////////// SceneNode
	class SceneNode
	{
	public:
		SceneNode();
		SceneNode(SceneNode* parent);
		~SceneNode();
		SceneNode* createNode();

		void addNode(SceneNode* node);
		
		void setMesh(Mesh* mesh);
		void setMatrix(Mat4& matrix);
		void setParent(SceneNode* parent);
		void setCallback(ISceneNodeCallBack* callback);
		void setShaderProgram(ShaderProgram* shader);

		std::vector<SceneNode*> getNodes();
		Mesh* getMesh();
		Mat4 getModelMatrix();
		ShaderProgram* getActiveShaderProgram();

		void draw();


	private:
		SceneNode* parent;
		std::vector<SceneNode*> nodes;
		ShaderProgram* shader;
		Mesh* mesh;
		Mat4 matrix;
		ISceneNodeCallBack* callback;	

	};
}