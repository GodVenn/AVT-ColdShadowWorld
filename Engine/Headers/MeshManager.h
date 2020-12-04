#pragma once

#include "Singleton.h"
#include "IManager.h"
#include "Mesh.h"
namespace engine
{
	class MeshManager : public Singleton<MeshManager>, public IManager<Mesh>
	{
		friend Singleton<MeshManager>;
		MeshManager();
		~MeshManager();

	public:
		virtual void remove(std::string meshName) override;
		virtual Mesh* get(std::string meshName) override;
		virtual void add(std::string meshName, Mesh* mesh) override;

	};
}